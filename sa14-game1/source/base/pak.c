#include "pak.h"

#include "base/array.h"
#include "base/common.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define PakMagicNumber (0xa2c5f1b4)

#pragma pack(push, 1)
typedef struct {
    int    magic_number;
    short  version;
    int    time;
    string name[16];
    int    num_files;
} pakArchiveHeaderT;
#pragma pack(pop)

struct pakArchiveT {
    pakArchiveHeaderT header;

    string** file_names;

    FILE* fp;
    string* password;
};

#pragma pack(push, 1)
typedef struct {
    string name[64];
    int    size;
    int    crc32;
} pakFileHeaderT;
#pragma pack(pop)

struct pakFileT {
    pakArchiveT* pak;

    int base;
    int size;

    int pos;
};

static void decrypt(void* data, size_t count, const string* password, int n) {
    int pw_len = strlen(password);

    int pw = 0;
    for (int i = 0; i < pw_len; i++)
        pw += password[i]*(i+1)*251;

    for (int i = 0; i < (int)count; i++)
        ((uint8_t*)data)[i] ^= (pw_len+pw*983 + password[(i+n) % pw_len] + (((i+n)+1)*3163)) & 0xff;
}

static bool isValidPakArchiveHeader(const pakArchiveHeaderT* pak) {
    if (pak->magic_number != PakMagicNumber)
        return (false);

    return (true);
}

static bool readPakArchiveHeader(pakArchiveT* pak) {
    if (fread(&pak->header, sizeof(pakArchiveHeaderT), 1, pak->fp) != 1) {
        fclose(pak->fp);
        return (false);
    }

    if (pak->password)
        decrypt(&pak->header, sizeof(pakArchiveHeaderT), pak->password, 0);

    if (!isValidPakArchiveHeader(&pak->header)) {
        fclose(pak->fp);
        return (false);
    }

    return (true);
}

pakArchiveT* pakOpenArchive(const string* file_name, const string* password) {
    pakArchiveT* pak = malloc(sizeof(pakArchiveT));

    pak->fp = fopen(file_name, "rb");

    if (!pak->fp) {
        free(pak);
        return (NULL);
    }

    pak->password = strdup(password);

    assert(readPakArchiveHeader(pak));

    pak->file_names = malloc(sizeof(string*) * pak->header.num_files);

    int i = 0;
    while (!feof(pak->fp)) {
        pakFileHeaderT pak_file;
        if (fread(&pak_file, sizeof(pakFileHeaderT), 1, pak->fp) != 1) {
            // Premature EOF.
            break;
        }

        if (pak->password)
            decrypt(&pak_file, sizeof(pakFileHeaderT), pak->password, 0);

        pak->file_names[i++] = strdup(pak_file.name);

        fseek(pak->fp, pak_file.size, SEEK_CUR);
    }

    assert(i == pak->header.num_files);

    return (pak);
}

void pakCloseArchive(pakArchiveT* pak) {
    fclose(pak->fp);

    for (int i = 0; i < pak->header.num_files; i++)
        free(pak->file_names[i]);

    free(pak->file_names);
    free(pak->password);
    free(pak);
}

int pakNumFiles(pakArchiveT* pak) {
    return (pak->header.num_files);
}

const string* pakGetFilename(pakArchiveT* pak, int i) {
    assert(0 <= i && i < pak->header.num_files);

    return (pak->file_names[i]);
}

pakFileT* pakOpenFile(pakArchiveT* pak, const string* file_name) {
    fseek(pak->fp, sizeof(pakArchiveHeaderT), SEEK_SET);

    while (!feof(pak->fp)) {
        pakFileHeaderT pak_file;
        if (fread(&pak_file, sizeof(pakFileHeaderT), 1, pak->fp) != 1) {
            // Premature EOF.
            break;
        }

        if (pak->password)
            decrypt(&pak_file, sizeof(pakFileHeaderT), pak->password, 0);

        if (strcmp(pak_file.name, file_name) == 0) {
            pakFileT* pf = malloc(sizeof(pakFileT));

            pf->pak  = pak;
            pf->base = ftell(pak->fp);
            pf->size = pak_file.size;
            pf->pos  = 0;

            return (pf);
        }

        fseek(pak->fp, pak_file.size, SEEK_CUR);
    }

    return (NULL);
}

void pakCloseFile(pakFileT* pak_file) {
    free(pak_file);
}

int pakFileSize(const pakFileT* pak_file) {
    return (pak_file->size);
}

int pakFileSeek(pakFileT* pak_file, size_t n) {
    pak_file->pos += n;

    if (pak_file->pos < 0)
        pak_file->pos = 0;

    if (pak_file->pos > pak_file->size)
        pak_file->pos = pak_file->size;

    return (pak_file->pos);
}

bool pakEOF(const pakFileT* pak_file) {
    return (pak_file->pos >= pak_file->size);
}

int pakRead(pakFileT* pak_file, char* buf, size_t count) {
    size_t max_count = pak_file->size - pak_file->pos;

    if (max_count == 0)
        return (0);

    if (count > max_count)
        count = max_count;

    fseek(pak_file->pak->fp, pak_file->base + pak_file->pos, SEEK_SET);
    count = fread(buf, sizeof(char), count, pak_file->pak->fp);

    if (pak_file->pak->password)
        decrypt(buf, count, pak_file->pak->password, pak_file->pos);

    pak_file->pos += count;

    return (count);
}

uint8_t* pakReadFile(pakArchiveT* pak, const string* file_name) {
    pakFileT* pf = pakOpenFile(pak, file_name);

    if (!pf)
        return (NULL);

    // +1 because we append a null-char just in case it's a text file we're
    // reading. It doesn't affect binary formats anyway.
    uint8_t* buf = malloc(pf->size+1);
    if (!buf) {
        pakCloseFile(pf);
        return (NULL);
    }

    pakRead(pf, buf, pf->size);
    buf[pf->size] = '\0';

    pakCloseFile(pf);

    return (buf);
}

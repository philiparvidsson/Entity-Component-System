#ifndef pak_h_
#define pak_h_

#include "base/common.h"

#include <stdint.h>

typedef struct pakArchiveT pakArchiveT;
typedef struct pakFileT pakFileT;

pakArchiveT* pakOpenArchive(const string* file_name, const string* password);
void pakCloseArchive(pakArchiveT* pak);

int pakNumFiles(pakArchiveT* pak);
const string* pakGetFilename(pakArchiveT* pak, int i);

pakFileT* pakOpenFile(pakArchiveT* pak, const string* file_name);
void pakCloseFile(pakFileT* pak_file);
int pakFileSize(const pakFileT* pak_file);
int pakFileSeek(pakFileT* pak_file, size_t n);
bool pakEOF(const pakFileT* pak_file);
int pakRead(pakFileT* pak_file, char* buf, size_t count);
uint8_t* pakReadFile(pakArchiveT* pak, const string* file_name);

#endif // pak_h_

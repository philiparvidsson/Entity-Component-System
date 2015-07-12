#include "texture.h"

#include "base/common.h"
#include "base/file_io.h"

struct textureT {
    int lol;
};

textureT* loadTextureBMP(const void* bmp_data) {

}

textureT* loadTextureFileBMP(const void* file_name) {
    void* bmp_data = readFile(file_name);
    textureT* tex = loadTextureBMP(bmp_data);
    free(bmp_data);
    return (tex);
}

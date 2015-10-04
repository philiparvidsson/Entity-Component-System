#ifdef __linux__

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"
#include "base/debug.h"
#include "engine/game.h"
#include "graphics/graphics.h"
#include "graphics/shader.h"
#include "graphics/text.h"
#include "graphics/trimesh.h"

#include <stddef.h>
#include <stdint.h>

#include <GL/glew.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void drawText(const string* text, float x, float y, const string* font_name, int font_size) {

}

void loadFontFromFile(const string* file_name) {
}

void loadFontFromMemory(void* font_data, int num_bytes) {
}

#endif // __linux__

#ifndef shader_h_
#define shader_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: shaderT
 *
 * Description:
 *   Represents a GLSL shader program.
 *------------------------------------*/
typedef struct shaderT shaderT;

typedef enum {
    IntUniform,
    FloatUniform,
    Matrix4Uniform
} uniformTypeT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

shaderT *createShader(void);
void deleteShader(shaderT *shader);

void compileFragmentShader(shaderT *shader, string const *source);
void compileGeometryShader(shaderT *shader, string const *source);
void compileVertexShader(shaderT *shader, string const *source);

void setShaderUniform(string const *name, uniformTypeT type, const void *value);
void useShader(shaderT const *shader);

#endif // shader_h_

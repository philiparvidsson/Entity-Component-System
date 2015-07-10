#ifndef shader_h_
#define shader_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"

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

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

shaderT* createShader(void);
void deleteShader(shaderT* shader);

void compileFragmentShader(shaderT* shader, const string* source);
void compileGeometryShader(shaderT* shader, const string* source);
void compileVertexShader(shaderT* shader, const string* source);

void setShaderParam(const string* name, const void* value);
shaderT* useShader(const shaderT* shader);

void shaderPostProcess(const shaderT* shader);

#endif // shader_h_

/*------------------------------------------------------------------------------
 * File: sa14-game1.c
 * Created: June 7, 2015
 * Last changed: June 9, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Huvudprogram för spelet. Vad nu det innebär. Det får tiden utvisa. :-)
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"
#include "core/linmath.h"

#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: printIntroMessage()
 *
 * Description:
 *   Skriver ut introduktionsmeddelandet.
 *------------------------------------*/
static void printIntroMessage(void) {
    printf("sa14-game1 v%s by %s\n\n\n", ProgramVersion, ProgramAuthors);
    printf("Some time in the not too distant future, this program will be a\n"
           "really cool game instead of this crap that does nothing...\n");
}

/*--------------------------------------
 * Function: main()
 *
 * Description:
 *   Programmets huvudfunktion.
 *------------------------------------*/
int main(void) {
    printIntroMessage();

    initGraphics("Main Window", 640, 640);
    setFrameRate(60.0f);

    shaderProgramADT shader_program = createShaderProgram();

    compileVertexShader(shader_program, readFile("shaders/test_shader.vert"));
    compileFragmentShader(shader_program, readFile("shaders/test_shader.frag"));

    geometryT *box = createBox(0.1f, 0.2f, 0.4f);

    mat4x4 la = mat4x4_lookAt(
        (vec3) { 0.0f, 0.5f, -1.0f },
        (vec3) { 0.0f, 0.0f, 0.0f },
        (vec3) { 0.0f, 1.0f, 0.0f }
    );

    mat4x4 o = mat4x4_perspective(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, -3.0f);
    //o = mat4x4_identity();
    //cm = identityMatrix4();

    mat4x4 view_matrix = o;// mat4x4_mul(o, la);

    float ff = 0.0f;
    while (windowIsOpen()) {
        clearDisplay(0.0f, 0.0f, 0.4f);
        ff += 0.25 / 60.0f;

        useShaderProgram(shader_program);
        setShaderUniform("SomeVal", FloatUniform, &ff);
        setShaderUniform("ViewMatrix", Matrix4Uniform, &view_matrix);

        drawGeometry(box);
        
        updateDisplay();
    }

    deleteGeometry(box);
    deleteShaderProgram(shader_program);

    exitGraphics();
}

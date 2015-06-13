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

#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>

#include <Windows.h>
#include "glew.h"
#include <GL/gl.h>

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

    initGraphics("Main Window", 640, 480);
    setFrameRate(60.0f);

    GLfloat vb[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vb), vb, GL_STATIC_DRAW);


    shaderProgramADT shader_program = createShaderProgram();

    loadVertexShader(shader_program, readFile("shaders/null.vert"));
    loadFragmentShader(shader_program, readFile("shaders/null.frag"));

    float ff = 0.0f;
    while (windowIsOpen()) {
        clearDisplay(0.0f, 0.0f, 0.4f);
        ff += 0.25 / 60.0f;
        setShaderParam(shader_program, "some_val", ff);
        useShaderProgram(shader_program);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
        
        updateDisplay();
    }

    deleteShaderProgram(shader_program);

    exitGraphics();

    system("pause");
}

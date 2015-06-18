/*------------------------------------------------------------------------------
 * File: sa14-game1.c
 * Created: June 7, 2015
 * Last changed: June 9, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Huvudprogram för spelet. Vad nu det innebär. Det får tiden utvisa. :-)
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/
#if 1
#include "core/common.h"

#include "graphics.h"
#include "graphics/meshgen.h"

#include "math/matrix.h"

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

    initGraphics("Lol Window", 640, 640);
    setFrameRate(60.0f);

    shaderProgramADT shader_program = createShaderProgram();

    compileVertexShader(shader_program, readFile("shaders/test_shader.vert"));
    compileFragmentShader(shader_program, readFile("shaders/test_shader.frag"));

    triMeshT *box1 = createCone(0.1f, 0.2f, 8);

    mat4x4 proj = mat4x4_perspective(-1.0f, 1.0f, -1.0f, 1.0f, -0.1f, -3.0f);
    mat4x4 view;
    
    mat4x4_look_at(
        &(vec3) { 0.0f, 0.5f, 1.0f },
        &(vec3) { 0.0f, 0.0f, 0.0f },
        &(vec3) { 0.0f, 1.0f, 0.0f },
        &view
    );

/*view = mat4x4_lookAt(
        (vec3) { 0.0f, 0.5f, 1.0f },
        (vec3) { 0.0f, 0.0f, 0.0f },
        (vec3) { 0.0f, 1.0f, 0.0f });*/

    float ff = 0.0f;
    while (windowIsOpen()) {
        mat4x4 rot_x, rot_y, rot_z;

        mat_rot_x(ff*0.5f, &rot_x);
        mat_rot_y(ff*1.3f, &rot_y);
        mat_rot_z(ff*0.7f, &rot_z);

        mat4x4 t;

        mat_identity(&t);
        mat_mul(&rot_y, &t, &t);
        mat_mul(&rot_z, &t, &t);
        mat_mul(&rot_x, &t, &t);

        clearDisplay(0.0f, 0.0f, 0.4f);
        ff += 0.75f / 60.0f;

        useShaderProgram(shader_program);
        setShaderUniform("View", Matrix4Uniform, &view);
        setShaderUniform("Proj", Matrix4Uniform, &proj);

        setShaderUniform("Model", Matrix4Uniform, &t);
        drawMesh(box1);

        
        updateDisplay();
    }

    deleteMesh(box1);
    deleteShaderProgram(shader_program);

    exitGraphics();
}

#endif
#if 0
#include "math/matrix.h"
#include "math/vector.h"
#include <xmmintrin.h>
int main(void) {
    //mat4x4 m, m2;


    //mat_identity(&m2);


    //m = m2;

    /*printf("%f\t%f\t%f\t%f\n", m.x.x, m.x.y, m.x.z, m.x.w);
    printf("%f\t%f\t%f\t%f\n", m.y.x, m.y.y, m.y.z, m.y.w);
    printf("%f\t%f\t%f\t%f\n", m.z.x, m.z.y, m.z.z, m.z.w);
    printf("%f\t%f\t%f\t%f\n", m.w.x, m.w.y, m.w.z, m.w.w);*/

    vec4 m = { 1.0f, 2.0f, 4.0f, 8.0f };
    vec4 m2 = { 4.0f, 8.0f, 16.0f, 32.0f };

    vec_add(&m, &m2, &m);
    vec_scale(&m, 0.5f, &m);
    

    printf("%f\t%f\t%f\t%f\n", m.x, m.y, m.z, m.w);
    vec_normalize(&m, &m);
    printf("%f\t%f\t%f\t%f\n", m.x, m.y, m.z, m.w);


    system("pause");
    return 0;
}
#endif

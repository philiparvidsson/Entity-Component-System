/*------------------------------------------------------------------------------
 * File: sa14-game1.c
 * Created: June 7, 2015
 * Last changed: June 9, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Huvudprogram f�r spelet. Vad nu det inneb�r. Det f�r tiden utvisa. :-)
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"
#include "base/file_io.h"
#include "base/time.h"

#include "engine/game.h"

#include "graphics/graphics.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/trimesh.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: printIntroMessage()
 *
 * Description:
 *   Prints an introductory message.
 *
 * Usage:
 *   printIntroMessage();
 *------------------------------------*/
static void printIntroMessage(void) {
    printf("sa14-game1 v%s by %s\n\n\n", ProgramVersion, ProgramAuthors);
    printf("Some time in the not too distant future, this program will be a\n"
           "really cool game instead of this crap that does nothing...\n\n");

    printf("Full source available @ github.com/philiparvidsson\n\n");
}


/*--------------------------------------
 * Function: showSplashScreen(tex, secs)
 * Parameters:
 *   tex   The logo texture.
 *   secs  The time to display the logo, in seconds.
 *
 * Description:
 *   Shows a splash screen for the specfied amount of time.
 *
 * Usage:
 *   showSplashScreen(my_tex, 3.0f);
 *------------------------------------*/
static void showSplashScreen(textureT* splash_tex, float secs) {
    string* vert_src = readFile("resources/shaders/discard_z.vert");
    string* frag_src = readFile("resources/shaders/splashscreen.frag");

    shaderT* splash_shader = createShader();
    triMeshT* quad = createQuad(2.0f, 2.0f);

    compileVertexShader(splash_shader, vert_src);
    compileFragmentShader(splash_shader, frag_src);

    useShader(splash_shader);
    useTexture(splash_tex, 0);

    free(vert_src);
    free(frag_src);

    timeT time = getTime();
    while (windowIsOpen()) {
        float elapsed = elapsedSecsSince(time);

        if (elapsed >= secs)
            break;

        // Default is fade=1.0, which gives a final value of 0.0, which is
        // 100% texture color and no fade.
        float fade = 1.0f;

        // Fade in the first second...
        if (elapsed < 1.0f)
            fade = sin(0.5f*elapsed*3.141592653);

        // ...and fade out the last 0.5 seconds.
        float time_left = secs - elapsed;
        if (time_left < 0.5f)
            fade = sin(time_left*3.141592653);

        // We multiply the fade value by itself to get sin(x)^2.
        fade *= fade;

        // Invert it to get the correct value.
        fade = 1.0f - fade;

        clearDisplay(1.0f, 0.0f, 1.0f);
        setShaderParam("Fade", &fade);
        drawMesh(quad);
        updateDisplay();
    }

    freeMesh(quad);

    useTexture(NULL, 0);
    useShader(NULL);
    deleteShader(splash_shader);
}

/*--------------------------------------
 * Function: main()
 *
 * Description:
 *   Programmets huvudfunktion.
 *------------------------------------*/
int main(void) {
    printIntroMessage();

    initGame("Asteroids", 1280, 720);

    textureT* tex = loadTextureFromFile("resources/images/splash1.bmp");
    showSplashScreen(tex, 3.0f);
    freeTexture(tex);

    addSubsystemToGame(newPhysicsSubsystem());
    addSubsystemToGame(newGraphicsSubsystem());

    gameEntityT* player_entity = newPlayerEntity();
    addEntityToGame(player_entity);

    gameMain();
}

/*------------------------------------------------------------------------------
 * File: graphics.h
 * Created: June 8, 2015
 * Last changed: June 13, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder ett enkelt grafikbibliotek f�r att rita och visa grafik. Biblio-
 *   teket anv�nder OpenGL och renderar s�ledes med h�rdvara om m�jlighet finns.
 *
 *----------------------------------------------------------------------------*/

#ifndef graphics_h_
#define graphics_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"
#include "core/math.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    int lol;
} lightSourceT;

/*--------------------------------------
 * Type: triT
 *
 * Description:
 *   Representerar en triangel i en geometri.
 *------------------------------------*/
typedef struct {
    int v0, v1, v2;
} triT;

/*--------------------------------------
 * Type: geometryT
 *
 * Description:
 *   Representerar en bit geometri.
 *------------------------------------*/
typedef struct {
          vector3T *const verts;
          vector3T *const normals;
    const int             num_verts;
          triT     *const tris;
    const int             num_tris;
} geometryT;

/*--------------------------------------
 * Type: shaderProgramADT
 *
 * Description:
 *   Abstrakt pekartyp f�r ett shader-program.
 *------------------------------------*/
typedef struct shaderProgramCDT *shaderProgramADT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*------------------------------------------------------------------------------
 * Funktioner f�r att initiera grafikl�ge, avsluta grafikl�ge samt konfigurera
 * grafikl�get och st�lla in parametrar.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: initGraphics()
 * Parameters:
 *   title   F�nstrets titel.
 *   width   F�nstrets bredd, i antal pixlar.
 *   height  F�nstrets h�jd, i antal pixlar.
 *
 * Description:
 *   Skapar ett grafikf�nster med de specificerade dimensionerna. Den angivna
 *   bredden och h�jden inkluderar inte f�nsterdekorationer, utan endast storleken
 *   storleken p� klientytan.
 *------------------------------------*/
void initGraphics(string title, int width, int height);

/*--------------------------------------
 * Function: exitGraphics()
 * Parameters:
 *
 * Description:
 *   St�nger grafikf�nstret.
 *------------------------------------*/
void exitGraphics(void);

/*--------------------------------------
 * Function: createShaderProgram()
 * Parameters:
 *
 * Returns:
 *   En pekare till ett shader-program.
 *
 * Description:
 *   Skapar ett nytt shader-program.
 *------------------------------------*/
shaderProgramADT createShaderProgram(void);

/*--------------------------------------
 * Function: compileFragmentShader()
 * Parameters:
 *   program  Det shader-program som shadern ska l�ggas till i.
 *   source   GLSL-koden som ska kompileras och l�nkas till det specificerade
 *            shader-programmet.
 *
 * Description:
 *   Kompilerar och l�nkar GLSL-kod till en fragment-shader i det specificerade
 *   shader-programmet.
 *------------------------------------*/
void compileFragmentShader(shaderProgramADT program, string source);

/*--------------------------------------
 * Function: compileVertexShader()
 * Parameters:
 *   program  Det shader-program som shadern ska l�ggas till i.
 *   source   GLSL-koden som ska kompileras och l�nkas till det specificerade
 *            shader-programmet.
 *
 * Description:
 *   Kompilerar och l�nkar GLSL-kod till en vertex-shader i det specificerade
 *   shader-programmet.
 *------------------------------------*/
void compileVertexShader(shaderProgramADT program, string source);

/*--------------------------------------
 * Function: setShaderUniform()
 * Parameters:
 *   program  Det shader-program vars parametrar ska st�llas in.
 *   name     Namnet p� uniform-variabeln.
 *   val      V�rdet som uniform-variabeln ska tilldelas.
 *
 * Description:
 *   S�tter den specificerade uniform-variabeln till det specificerade v�rdet.
 *   Se nyckelordet uniform i spr�kspecifikationen f�r GLSL f�r mer information.
 *------------------------------------*/
void setShaderUniform(shaderProgramADT program, string name, float value);

/*--------------------------------------
 * Function: useShaderProgram()
 * Parameters:
 *   program  Det shader-program som ska anv�ndas f�r n�stkommande ritoperationer.
 *
 * Description:
 *   Aktiverar det specificerade shader-programmet.
 *------------------------------------*/
void useShaderProgram(shaderProgramADT program);

/*--------------------------------------
 * Function: deleteShaderProgram()
 * Parameters:
 *   program  Det shader-program som ska tas bort.
 *
 * Description:
 *   Tar bort det specificerade shader-programmet inklusive alla tillh�rande
 *   shaders.
 *------------------------------------*/
void deleteShaderProgram(shaderProgramADT program);

/*--------------------------------------
 * Function: setFrameRate()
 * Parameters:
 *
 * Description:
 *   St�ller in hur m�nga bildrutor som ska visas per sekund. Ange noll f�r
 *   obegr�nsat antal.
 *------------------------------------*/
void setFrameRate(float fps);

/*------------------------------------------------------------------------------
 * Funktioner f�r att rensa ritytan, rita geomtriska figurer och texturer samt
 * f�r att presentera ritytan p� sk�rmen m.m.
 *----------------------------------------------------------------------------*/

geometryT *createBox(float width, float height, float length);

/*--------------------------------------
 * Function: deleteGeometry()
 * Parameters:
 *   geom Den geometri som ska tas bort.
 *
 * Description:
 *   Tar bort den specificerade geometrin.
 *------------------------------------*/
void deleteGeometry(geometryT *geom);

/*--------------------------------------
 * Function: clearDisplay()
 * Parameters:
 *   r  R�d f�rgkomponent.
 *   g  Gr�n f�rgkomponent.
 *   b  Bl� f�rgkomponent.
 *
 * Description:
 *   Rensar ritytan til den specificerade f�rgen.
 *------------------------------------*/
void clearDisplay(float r, float g, float b);

/*--------------------------------------
 * Function: setColor()
 * Parameters:
 *   r  R�d f�rgkomponent.
 *   g  Gr�n f�rgkomponent.
 *   b  Bl� f�rgkomponent.
 *   a  Alpha-v�rde.
 *
 * Description:
 *   �ndrar f�rg f�r n�stkommande anrop till ritfunktioner.
 *------------------------------------*/
void setColor(float r, float g, float b, float a);

void drawGeometry(const geometryT *geom);

/*--------------------------------------
 * Function: updateDisplay()
 * Parameters:
 *
 * Description:
 *   Uppdaterar det grafikf�nstret.
 *------------------------------------*/
void updateDisplay(void);

/*------------------------------------------------------------------------------
 * Funktioner f�r att l�sa ut inst�llningar och information om grafikl�get m.m.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: windowIsOpen()
 * Parameters:
 *
 * Returns:
 *   Sant om grafikf�nstret �r �ppet, annars falskt.
 *
 * Description:
 *   Returnerar sant om grafikf�nstret �r �ppet.
 *------------------------------------*/
bool windowIsOpen(void);

#endif /* graphics_h_ */

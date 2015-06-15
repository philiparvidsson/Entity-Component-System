/*------------------------------------------------------------------------------
 * File: graphics.h
 * Created: June 8, 2015
 * Last changed: June 14, 2015
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
#include "core/linmath.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

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
 *   Represents a piece of geometry with a vertex mesh etc.
 *------------------------------------*/
typedef struct {
          vec3   *const verts;      /* The vertices.           */
          vec3   *const normals;    /* The vertex normals.     */
    const int           num_verts;  /* Number of vertices.     */
          triT   *const tris;       /* The triangles (faces).  */
    const int           num_tris;   /* Number of triangles.    */
          mat4x4        transform; /* Model transform matrix. */
} geometryT;

/*--------------------------------------
 * Type: shaderProgramADT
 *
 * Description:
 *   Abstrakt pekartyp f�r ett shader-program.
 *------------------------------------*/
typedef struct shaderProgramCDT *shaderProgramADT;

typedef enum {
    FloatUniform,
    Matrix4Uniform
} uniformTypeT;

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
 *   name   Namnet p� uniform-variabeln.
 *   type   Uniform-variabelns datatype.
 *   value  V�rdet som uniform-variabeln ska tilldelas.
 *
 * Description:
 *   S�tter den specificerade uniform-variabeln till det specificerade v�rdet.
 *   Se nyckelordet uniform i spr�kspecifikationen f�r GLSL f�r mer information.
 *------------------------------------*/
void setShaderUniform(string name, uniformTypeT type, const void *value);

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

/*--------------------------------------
* Function: createBox()
* Parameters:
*   width   L�dans bredd (x).
*   height  L�dans h�jd (y).
*   length  L�dans l�ngd (z).
*
* Returns:
*   En pekare till l�dans geometri.
*
* Description:
*   Skapar geometrin f�r en l�dform.
*------------------------------------*/
geometryT *createBox(float width, float height, float length);

/*--------------------------------------
 * Function: updateGeometry()
 * Parameters:
 *   geom Den geometri som ska uppdateras.
 *
 * Description:
 *   Uppdaterar geometridatan i GPU:n genom att ladda upp den p� nytt.
 *------------------------------------*/
void updateGeometry(const geometryT *geom);

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
 * Function: drawGeometry()
 * Parameters:
 *   geom  Geometrin som ska ritas upp.
 *
 * Description:
 *   Ritar den specificerade geometrin.
 *------------------------------------*/
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

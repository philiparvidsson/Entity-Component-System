/*------------------------------------------------------------------------------
 * File: graphics.h
 * Created: June 8, 2015
 * Last changed: June 14, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder ett enkelt grafikbibliotek för att rita och visa grafik. Biblio-
 *   teket använder OpenGL och renderar således med hårdvara om möjlighet finns.
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
 *   Abstrakt pekartyp för ett shader-program.
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
 * Funktioner för att initiera grafikläge, avsluta grafikläge samt konfigurera
 * grafikläget och ställa in parametrar.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: initGraphics()
 * Parameters:
 *   title   Fönstrets titel.
 *   width   Fönstrets bredd, i antal pixlar.
 *   height  Fönstrets höjd, i antal pixlar.
 *
 * Description:
 *   Skapar ett grafikfönster med de specificerade dimensionerna. Den angivna
 *   bredden och höjden inkluderar inte fönsterdekorationer, utan endast storleken
 *   storleken på klientytan.
 *------------------------------------*/
void initGraphics(string title, int width, int height);

/*--------------------------------------
 * Function: exitGraphics()
 * Parameters:
 *
 * Description:
 *   Stänger grafikfönstret.
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
 *   program  Det shader-program som shadern ska läggas till i.
 *   source   GLSL-koden som ska kompileras och länkas till det specificerade
 *            shader-programmet.
 *
 * Description:
 *   Kompilerar och länkar GLSL-kod till en fragment-shader i det specificerade
 *   shader-programmet.
 *------------------------------------*/
void compileFragmentShader(shaderProgramADT program, string source);

/*--------------------------------------
 * Function: compileVertexShader()
 * Parameters:
 *   program  Det shader-program som shadern ska läggas till i.
 *   source   GLSL-koden som ska kompileras och länkas till det specificerade
 *            shader-programmet.
 *
 * Description:
 *   Kompilerar och länkar GLSL-kod till en vertex-shader i det specificerade
 *   shader-programmet.
 *------------------------------------*/
void compileVertexShader(shaderProgramADT program, string source);

/*--------------------------------------
 * Function: setShaderUniform()
 * Parameters:
 *   name   Namnet på uniform-variabeln.
 *   type   Uniform-variabelns datatype.
 *   value  Värdet som uniform-variabeln ska tilldelas.
 *
 * Description:
 *   Sätter den specificerade uniform-variabeln till det specificerade värdet.
 *   Se nyckelordet uniform i språkspecifikationen för GLSL för mer information.
 *------------------------------------*/
void setShaderUniform(string name, uniformTypeT type, const void *value);

/*--------------------------------------
 * Function: useShaderProgram()
 * Parameters:
 *   program  Det shader-program som ska användas för nästkommande ritoperationer.
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
 *   Tar bort det specificerade shader-programmet inklusive alla tillhörande
 *   shaders.
 *------------------------------------*/
void deleteShaderProgram(shaderProgramADT program);

/*--------------------------------------
 * Function: setFrameRate()
 * Parameters:
 *
 * Description:
 *   Ställer in hur många bildrutor som ska visas per sekund. Ange noll för
 *   obegränsat antal.
 *------------------------------------*/
void setFrameRate(float fps);

/*------------------------------------------------------------------------------
 * Funktioner för att rensa ritytan, rita geomtriska figurer och texturer samt
 * för att presentera ritytan på skärmen m.m.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
* Function: createBox()
* Parameters:
*   width   Lådans bredd (x).
*   height  Lådans höjd (y).
*   length  Lådans längd (z).
*
* Returns:
*   En pekare till lådans geometri.
*
* Description:
*   Skapar geometrin för en lådform.
*------------------------------------*/
geometryT *createBox(float width, float height, float length);

/*--------------------------------------
 * Function: updateGeometry()
 * Parameters:
 *   geom Den geometri som ska uppdateras.
 *
 * Description:
 *   Uppdaterar geometridatan i GPU:n genom att ladda upp den på nytt.
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
 *   r  Röd färgkomponent.
 *   g  Grön färgkomponent.
 *   b  Blå färgkomponent.
 *
 * Description:
 *   Rensar ritytan til den specificerade färgen.
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
 *   Uppdaterar det grafikfönstret.
 *------------------------------------*/
void updateDisplay(void);

/*------------------------------------------------------------------------------
 * Funktioner för att läsa ut inställningar och information om grafikläget m.m.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: windowIsOpen()
 * Parameters:
 *
 * Returns:
 *   Sant om grafikfönstret är öppet, annars falskt.
 *
 * Description:
 *   Returnerar sant om grafikfönstret är öppet.
 *------------------------------------*/
bool windowIsOpen(void);

#endif /* graphics_h_ */

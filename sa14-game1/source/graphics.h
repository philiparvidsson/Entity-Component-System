/*------------------------------------------------------------------------------
 * File: graphics.h
 * Created: June 8, 2015
 * Last changed: June 16, 2015
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
 * Type: vertexT
 *
 * Description:
 *   Represents a single vertex in a triangle mesh.
 *------------------------------------*/
typedef struct {
    vec3 p; // The vertex position.
    vec3 n; // The vertex normal.
} vertexT;

/*--------------------------------------
 * Type: triT
 *
 * Description:
 *   Represents a single triangle (face) with vertex indices in a triangle mesh.
 *------------------------------------*/
typedef struct {
    int v0, v1, v2;
} triT;

/*--------------------------------------
 * Type: triMeshT
 *
 * Description:
 *   Represents a triangle mesh with vertices, normals, faces etc.
 *------------------------------------*/
typedef struct {
          vertexT *const verts;     // The vertices.
    const int            num_verts; // Number of vertices.
          triT    *const tris;      // The triangles (faces).
    const int            num_tris;  // Number of triangles.
} triMeshT;

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
 *   fps  The number of frames to display each second.
 *
 * Description:
 *   Sets the number of frames to display each second. Specify a framerate of
 *   zero to disable FPS synchronization.
 *
 * Usage:
 *   setFrameRate(60.0f);
 *------------------------------------*/
void setFrameRate(float fps);

/*------------------------------------------------------------------------------
 * Funktioner f�r att rensa ritytan, rita geomtriska figurer och texturer samt
 * f�r att presentera ritytan p� sk�rmen m.m.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: createBox()
 * Parameters:
 *   width   The width of the box (along the x-axis).
 *   height  The height of the box (y-axis).
 *   length  The length of the box (z-axis).
 *
 * Returns:
 *   A pointer to the generated mesh.
 *
 * Description:
 *   Creates a box mesh by laying out the vertices and attaching the triangles.
 *   The six sides of the box do not share vertices.
 *
 * Usage:
 *   triMeshT *cube = createBox(1.0f, 1.0f, 1.0f);
 *------------------------------------*/
triMeshT *createBox(float width, float height, float length);

/*--------------------------------------
 * Function: updateMesh()
 * Parameters:
 *   mesh  The mesh to update.
 *
 * Description:
 *   Updates the specified mesh. You should call this function after making any
 *   changes to the mesh data.
 *
 * Usage:
 *   updateMesh(my_mesh);
 *------------------------------------*/
void updateMesh(const triMeshT *mesh);

/*--------------------------------------
 * Function: deleteMesh()
 * Parameters:
 *   mesh  The mesh to delete.
 *
 * Description:
 *   Deletes the specified mesh.
 *
 * Usage:
 *   deleteMesh(my_mesh);
 *------------------------------------*/
void deleteMesh(triMeshT *mesh);

/*--------------------------------------
 * Function: clearDisplay()
 * Parameters:
 *   r  Red color component.
 *   g  Green color component.
 *   b  Blue color component..
 *
 * Description:
 *   Clears the display to the specified color.
 *
 * Usage:
 *   clearDisplay(1.0f, 1.0f, 1.0f);
 *------------------------------------*/
void clearDisplay(float r, float g, float b);

/*--------------------------------------
 * Function: drawMesh()
 * Parameters:
 *   mesh  The mesh to draw.
 *
 * Description:
 *   Draws the specified mesh. Remember to assign a shader program with the
 *   useShaderProgram() function.
 *
 * Usage:
 *   drawMesh(my_mesh);
 *------------------------------------*/
void drawMesh(const triMeshT *mesh);

/*--------------------------------------
 * Function: updateDisplay()
 * Parameters:
 *
 * Description:
 *   Updates the display by processing pending window events and swapping the
 *   graphics buffer into the graphics window. The function also waits a certain
 *   amount of time before returning, to maintain the FPS specified with the
 *   setFrameRate() function.
 *
 * Usage:
 *   updateDisplay();
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
 *   True if the graphics window is still open.
 *
 * Description:
 *   Returns true if the graphics window is open.
 *
 * Usage:
 *   bool is_open = windowIsOpen();
 *------------------------------------*/
bool windowIsOpen(void);

#endif /* graphics_h_ */

/*------------------------------------------------------------------------------
 * File: graphics.h
 * Created: June 8, 2015
 * Last changed: June 16, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   This file implements a graphics library using modern OpenGL, supporting
 *   shaders and much more, using a right-handed coordinate system.
 *----------------------------------------------------------------------------*/

#ifndef graphics_h_
#define graphics_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

#include "math/vector.h"

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
 *   Abstract pointer type for referencing shader programs.
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
 *   title   The window title.
 *   width   Window width, in pixels.
 *   height  Window height, in pixels.
 *
 * Description:
 *   Creates a graphics window with the specified dimensions. The dimensions do
 *   not include window decorations such as title bar, border etc, but rather
 *   specifies the size of the client area.
 *
 * Usage:
 *   initGraphics("Main Window", 640, 480);
 *------------------------------------*/
void initGraphics(string title, int width, int height);

/*--------------------------------------
 * Function: exitGraphics()
 * Parameters:
 *
 * Description:
 *   Closes the window and exits graphics mode.
 *
 * Usage:
 *   exitGraphics();
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
 * Funktioner för att rensa ritytan, rita geomtriska figurer och texturer samt
 * för att presentera ritytan på skärmen m.m.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: createMesh()
 * Parameters:
 *   num_verts  Number of vertices.
 *   num_tris   Number of triangles (faces).
 *
 * Returns:
 *   A pointer to the mesh.
 *
 * Description:
 *   Creates a new mesh with the specified number of vertices and triangles.
 *
 * Usage:
 *   triMeshT mesh = createMesh(8, 6);
 *------------------------------------*/
triMeshT *createMesh(int num_verts, int num_tris);

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
 * Functions for querying the graphics mode, settings etc.
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

#endif // graphics_h_

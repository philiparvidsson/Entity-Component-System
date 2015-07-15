/*------------------------------------------------------------------------------
 * File: graphics.h
 * Created: June 8, 2015
 * Last changed: June 20, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Functions for creating a single window, handling input etc.
 *----------------------------------------------------------------------------*/

#ifndef graphics_h_
#define graphics_h_

/*------------------------------------------------
 * INCLUDE
 *----------------------------------------------*/

#include "base/common.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void initGraphics(const string* title, int width, int height);
void exitGraphics(void);
void clearDisplay(float r, float g, float b);
void updateDisplay(void);

int screenWidth(void);
int screenHeight(void);

bool windowIsFocused(void);
bool windowIsOpen(void);

#endif // graphics_h_

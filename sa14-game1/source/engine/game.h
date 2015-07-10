#ifndef game_h_
#define game_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

typedef struct entityT entityT;
typedef struct gameT gameT;
typedef struct subsystemT subsystemT;

typedef void (*entityUpdateFuncT)(entityT*, float);

typedef void (*gameFrameFuncT)(float);

#include "base/common.h"

#include "engine/entity.h"

#include "input/keyboard.h"
#include "input/mouse.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void initGame(const string* title, int screen_width, int screen_height);
void exitGame(void);

void gameMain(void);
void gameAddEntity(entityT* e);
void gameRemoveEntity(entityT* e);
gameFrameFuncT gameGetFrameFunc(void);
void gameSetFrameFunc(gameFrameFuncT frame_func);

subsystemT* gameCreateSubsystem(const string* name);

#endif // game_h_

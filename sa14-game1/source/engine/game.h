#ifndef game_h_
#define game_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

typedef struct gameT gameT;

#include "base/common.h"
#include "base/pak.h"
#include "engine/component.h"
#include "engine/entity.h"
#include "engine/subsystem.h"
#include "input/keyboard.h"
#include "input/mouse.h"




/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void initGame(const string* title, int screen_width, int screen_height);
void exitGame(void);

void gameMain(void);

void addSubsystemToGame(gameSubsystemT* subsystem);
//void addEntityToGame(gameEntityT* entity);
void addGamePak(pakArchiveT* pak);
char* readGamePakFile(const string* file_name);
//void* delayedFree(void* mem);

#endif // game_h_

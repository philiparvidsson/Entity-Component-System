#ifndef game_h_
#define game_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

typedef struct entityT entityT;
typedef struct gameT gameT;

typedef void (*entityCleanupFuncT)(entityT*);
typedef void (*entityDrawFuncT)(entityT*);
typedef void (*entityUpdateFuncT)(entityT*);

typedef void (*gameFrameFuncT)(float);

#include "core/common.h"

#include "game/entity.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void gameMain(void);
void gameAddEntity(entityT* e);
gameFrameFuncT gameGetFrameFunc(void);
void gameSetFrameFunc(gameFrameFuncT frame_func);

#endif // game_h_

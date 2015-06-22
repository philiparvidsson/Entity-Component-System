/*------------------------------------------------------------------------------
 * File: time.h
 * Created: June 8, 2015
 * Last changed: June 20, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Provides functions for measure time with very high resolution.
 *----------------------------------------------------------------------------*/

#ifndef time_h_
#define time_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include <stdint.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef uint64_t timeT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

timeT getTime(void);

int elapsedMicrosecsSince(timeT time);

int elapsedMillisecsSince(timeT time);

float elapsedSecsSince(timeT time);

#endif // time_h_

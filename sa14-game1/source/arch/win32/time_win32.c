#ifdef WIN32

/*------------------------------------------------------------------------------
 * File: time_win32.c
 * Created: June 8, 2015
 * Last changed: October 4, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   Provides functions for measure time with very high resolution. We use
 *   QPC/QPF to accurately measure time using the internal system clock. QPC is
 *   not synchronized to any external time reference, but we only use it to
 *   measure time in a relative sense (time passed since...).
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"
#include "base/time.h"

#include <stdint.h>

#include <windows.h>

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

LARGE_INTEGER freq = { 0 };

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

timeT getTime(void) {
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);

    return ((timeT)count.QuadPart);
}

long long elapsedMicrosecsSince(timeT time) {
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);

    count.QuadPart -= time;

    if (freq.QuadPart == 0)
        QueryPerformanceFrequency(&freq);

    count.QuadPart *= 1000000;
    count.QuadPart /= freq.QuadPart;

    return (count.QuadPart);
}

int elapsedMillisecsSince(timeT time) {
    return (int)(elapsedMicrosecsSince(time) / 1000);
}

float elapsedSecsSince(timeT time) {
    return (elapsedMicrosecsSince(time) / 1000000.0f);
}

#endif // WIN32

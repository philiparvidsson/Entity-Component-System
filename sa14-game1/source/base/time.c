/*------------------------------------------------------------------------------
 * File: time.c
 * Created: June 8, 2015
 * Last changed: June 20, 2015
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

#include "time.h"

#include "base/common.h"

#include <stdint.h>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <sys/time.h>
#endif

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

#ifdef WIN32
LARGE_INTEGER freq = { 0 };
#endif

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

timeT getTime(void) {
#ifdef WIN32
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);

    return ((timeT)count.QuadPart);
#endif

#ifdef __linux__
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec*1000000) + tv.tv_usec);
#endif
}

long long elapsedMicrosecsSince(timeT time) {
#ifdef WIN32
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);

    count.QuadPart -= time;

    if (freq.QuadPart == 0)
        QueryPerformanceFrequency(&freq);

    count.QuadPart *= 1000000;
    count.QuadPart /= freq.QuadPart;

    return (count.QuadPart);
#endif

#ifdef __linux__
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec*1000000) + tv.tv_usec - time);
#endif
}

int elapsedMillisecsSince(timeT time) {
    return (int)(elapsedMicrosecsSince(time) / 1000);
}

float elapsedSecsSince(timeT time) {
    return (elapsedMicrosecsSince(time) / 1000000.0f);
}

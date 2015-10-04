#ifdef __linux__

/*------------------------------------------------------------------------------
 * File: time_linux.c
 * Created: October 4, 2015
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

#include <sys/time.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

timeT getTime(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec*1000000) + tv.tv_usec);
}

long long elapsedMicrosecsSince(timeT time) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec*1000000) + tv.tv_usec - time);
}

int elapsedMillisecsSince(timeT time) {
    return (int)(elapsedMicrosecsSince(time) / 1000);
}

float elapsedSecsSince(timeT time) {
    return (elapsedMicrosecsSince(time) / 1000000.0f);
}

#endif // __linux__

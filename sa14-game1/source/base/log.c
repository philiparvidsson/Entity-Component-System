#include "log.h"

#include "base/common.h"

#include <windows.h>

static void setConsoleColor() {
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hstdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

void logInfo(const string* msg) {
    setConsoleColor();
    printf("%s\n", msg);
}

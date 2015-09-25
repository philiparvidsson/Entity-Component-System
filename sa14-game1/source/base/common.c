#include "common.h"

#include <windows.h>

void sleep(int millisecs) {
    Sleep(millisecs);
}

wchar_t* wstrdup(const string* str) {
    size_t   len  = mbstowcs(NULL, str, 0)+1;
    wchar_t* wstr = malloc(sizeof(wchar_t) * len);

    mbstowcs(wstr, str, len);

    return (wstr);
}

int strcmpi2(const string* str1, const string* str2) {
    while (true) {
        int a = tolower(*(str1++));
        int b = tolower(*(str2++));

        if ((a == 0) || (a != b))
            return (a-b);
    }
}

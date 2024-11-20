#ifndef __TIME_H__
#define __TIME_H__

#include "log.h"

#ifdef _MSC_VER // 兼容微软平台
#include <windows.h>
#include <WinSock.h>
static int gettimeofday(struct timeval *tp, void *tzp) {
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    tm.tm_year = wtm.wYear - 1900;
    tm.tm_mon = wtm.wMonth - 1;
    tm.tm_mday = wtm.wDay;
    tm.tm_hour = wtm.wHour;
    tm.tm_min = wtm.wMinute;
    tm.tm_sec = wtm.wSecond;
    tm.tm_isdst = -1;
    clock = mktime(&tm);
    tp->tv_sec = clock;
    tp->tv_usec = wtm.wMilliseconds * 1000;
    return (0);
}
#else

#include <sys/time.h>

#endif // _MSC_VER

static unsigned long long currentTimeMillis(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000ULL + tv.tv_usec;
}

#ifdef DEBUG
#define __TIC1__(tag) unsigned long long time_##tag##_start = currentTimeMillis();
#define __TOC1__(tag) unsigned long long time_##tag##_end = currentTimeMillis();\
        LOG_D(#tag " time: %.3f ms", ((time_##tag##_end - time_##tag##_start) / 1000.0));
#else
#define __TIC1__(tag)
#define __TOC1__(tag)
#endif // DEBUG

#endif //__TIME_H__

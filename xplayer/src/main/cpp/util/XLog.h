//
// Created by 24415 on 2024-11-01.
//

#ifndef FFMPEG_XLOG_H
#define FFMPEG_XLOG_H

#ifndef LOG_TAG
#define LOG_TAG "XPlayer"
#endif

#ifdef ANDROID

#include <android/log.h>

#define XLOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,  __VA_ARGS__)
#define XLOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG,  __VA_ARGS__)
#define XLOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,  __VA_ARGS__)

#else

#define XLOGD(...) printf(TAG,  __VA_ARGS__)
#define XLOGI(...) printf(TAG,  __VA_ARGS__)
#define XLOGE(...) printf(TAG,  __VA_ARGS__)

#endif

#endif //FFMPEG_XLOG_H

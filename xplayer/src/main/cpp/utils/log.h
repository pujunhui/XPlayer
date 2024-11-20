#ifndef __LOG_H__
#define __LOG_H__

#ifdef DEBUG

#include <string.h>

#ifdef _WIN32   //Windows32/64平台_WIN32都会被定义，而_WIN64只在64位windows上定义
#define __FILENAME__ (strrchr(__FILE__, '\\') + 1) // Windows下文件目录层级是'\\'
#else
#define __FILENAME__ (strrchr(__FILE__, '/') + 1) // Linux下文件目录层级是'/'，默认使用这种方式
#endif // _WIN32

#ifdef __ANDROID__ // Android平台

#ifndef LOG_TAG
#define LOG_TAG "JNI"
#endif

#include <android/log.h>

#define LOG_V(fmt, ...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG,\
        "[%s][%s][%d]: " fmt, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_D(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,\
        "[%s][%s][%d]: " fmt, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_I(fmt, ...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG,\
        "[%s][%s][%d]: " fmt, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_W(fmt, ...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG,\
        "[%s][%s][%d]: " fmt, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_E(fmt, ...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,\
        "[%s][%s][%d]: " fmt, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#else // 其它平台

#include <stdio.h>
#define LOG_V(fmt, ...) printf("[%s][%s][%d]: " fmt "\n",\
            __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_D(fmt, ...) printf("[%s][%s][%d]: " fmt "\n",\
            __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_I(fmt, ...) printf("[%s][%s][%d]: " fmt "\n",\
            __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_W(fmt, ...) printf("[%s][%s][%d]: " fmt "\n",\
            __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_E(fmt, ...) printf("[%s][%s][%d]: " fmt "\n",\
            __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif // __ANDROID__

#else // 如果没打开 DEBUG 开关，则只是一些空的语句

#define LOG_V(fmt, ...)
#define LOG_D(fmt, ...)
#define LOG_I(fmt, ...)
#define LOG_W(fmt, ...)
#define LOG_E(fmt, ...)

#endif // DEBUG
#endif // __LOG_H__

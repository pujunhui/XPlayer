//
// Created by 24415 on 2024-11-04.
//
#include <jni.h>
#include <android/native_window_jni.h>
#include <android/asset_manager_jni.h>

#include "utils/log.h"
#include "XPlayer.h"
#include "FFDecode.h"


static void jniThrowException(JNIEnv *env, const char *exception, const char *msg) {
//    jobject exception = env->NewObject(gStateExceptionFields.classId,
//                                       gStateExceptionFields.init, static_cast<int>(err),
//                                       env->NewStringUTF(msg));
//    env->Throw(static_cast<jthrowable>(exception));
}

static const char *const ClassName = "com/pujh/xplayer/XPlayer";

#ifndef NELEM
#define NELEM(x) (sizeof(x) / sizeof(x[0]))
#endif

struct fields_t {
    jfieldID context;
    jmethodID post_event;
};

static fields_t fields;

class JNIXPlayerListener : public XPlayerListener {
public:
    JNIXPlayerListener(JNIEnv *env, jobject thiz, jobject weak_thiz);

    ~JNIXPlayerListener();

    virtual void post_event(int msg, int ext1, int ext2);

private:
    JNIEnv *mEnv;
    jclass mClass;     // Reference to XPlayer class
    jobject mObject;    // Weak ref to XPlayer Java object to call on
};

JNIXPlayerListener::JNIXPlayerListener(JNIEnv *env, jobject thiz, jobject weak_thiz) {
    jclass clazz = env->GetObjectClass(thiz);
    if (clazz == NULL) {
        LOG_E("Can't find com/pujh/xplayer/XPlayer");
        jniThrowException(env, "java/lang/Exception", NULL);
        return;
    }
    mClass = (jclass) env->NewGlobalRef(clazz);

    // We use a weak reference so the XPlayer object can be garbage collected.
    // The reference is only used as a proxy for callbacks.
    mObject = env->NewGlobalRef(weak_thiz);
    mEnv = env;
}

JNIXPlayerListener::~JNIXPlayerListener() {
    // remove global references
    JNIEnv *env = mEnv;
    env->DeleteGlobalRef(mObject);
    env->DeleteGlobalRef(mClass);
}

void JNIXPlayerListener::post_event(int msg, int ext1, int ext2) {
    JNIEnv *env = mEnv;
    env->CallStaticVoidMethod(mClass, fields.post_event, mObject, msg, ext1, ext2);
    if (env->ExceptionCheck()) {
        LOG_W("An exception occurred while notifying an event.");
//        LOGW_EX(env);
        env->ExceptionClear();
    }
}

static void native_init(JNIEnv *env, jclass) {
    jclass clazz = env->FindClass(ClassName);
    if (clazz == NULL) {
        return;
    }

    fields.context = env->GetFieldID(clazz, "mNativeContext", "J");
    if (fields.context == NULL) {
        return;
    }

    fields.post_event = env->GetStaticMethodID(clazz, "postEventFromNative",
                                               "(Ljava/lang/Object;III)V");
    if (fields.post_event == NULL) {
        return;
    }
}

static XPlayer *setXPlayer(JNIEnv *env, jobject thiz, XPlayer *player) {
//    Mutex::Autolock l(sLock);
    XPlayer *old = (XPlayer *) env->GetLongField(thiz, fields.context);
    env->SetLongField(thiz, fields.context, (jlong) player);
    return old;
}

static XPlayer *getXPlayer(JNIEnv *env, jobject thiz) {
//    Mutex::Autolock l(sLock);
    return (XPlayer *) env->GetLongField(thiz, fields.context);
}

static void native_setup(JNIEnv *env, jobject thiz, jobject weak_this, jobject assetManager) {
    LOG_V("native_setup");
    XPlayer *player = new XPlayer();
    if (player == NULL) {
        jniThrowException(env, "java/lang/RuntimeException", "Out of memory");
        return;
    }
    JNIXPlayerListener *listener = new JNIXPlayerListener(env, thiz, weak_this);
    player->setListener(listener);

    AAssetManager *manager = AAssetManager_fromJava(env, assetManager);
    player->init(manager);

    setXPlayer(env, thiz, player);
}

static void native_release(JNIEnv *env, jobject thiz) {
    LOG_V("native_release");
    XPlayer *player = setXPlayer(env, thiz, NULL);
    if (player != NULL) {
        XPlayerListener *listener = player->setListener(NULL);
        delete listener;
        delete player;
    }
}

static void native_finalize(JNIEnv *env, jobject thiz) {
    LOG_V("native_finalize");
    XPlayer *player = getXPlayer(env, thiz);
    if (player != NULL) {
        LOG_W("XPlayer finalized without being released");
    }
    native_release(env, thiz);
}

static void native_setDataSource(JNIEnv *env, jobject thiz, jstring path) {
    XPlayer *player = getXPlayer(env, thiz);
    if (player == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }

    const char *path_c = env->GetStringUTFChars(path, NULL);
    LOG_V("native_setDataSource: path %s", path_c);
    player->setDataSource(path_c);
    env->ReleaseStringUTFChars(path, path_c);
}

static void native_setSurface(JNIEnv *env, jobject thiz, jobject surface) {
    XPlayer *player = getXPlayer(env, thiz);
    if (player == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }

    ANativeWindow *window = NULL;
    if (surface != NULL) {
        window = ANativeWindow_fromSurface(env, surface);
    }
    player->setSurface(window);
}

static void native_setDisplaySize(JNIEnv *env, jobject thiz, jint width, jint height) {
    XPlayer *player = getXPlayer(env, thiz);
    if (player == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    player->setDisplaySize(width, height);
}

static void native_setScaleType(JNIEnv *env, jobject thiz, jint scaleType) {
    XPlayer *player = getXPlayer(env, thiz);
    if (player == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    player->setScaleType(scaleType);
}

static void native_start(JNIEnv *env, jobject thiz) {
    XPlayer *player = getXPlayer(env, thiz);
    if (player == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    player->start();
}

static void native_stop(JNIEnv *env, jobject thiz) {
    XPlayer *player = getXPlayer(env, thiz);
    if (player == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    player->stop();
}

static void native_pause(JNIEnv *env, jobject thiz) {
    XPlayer *player = getXPlayer(env, thiz);
    if (player == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    player->pause();
}

static void native_resume(JNIEnv *env, jobject thiz) {
    XPlayer *player = getXPlayer(env, thiz);
    if (player == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    player->resume();
}

static void native_prepare(JNIEnv *env, jobject thiz) {
    XPlayer *player = getXPlayer(env, thiz);
    if (player == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    player->prepare();
}

static void native_reset(JNIEnv *env, jobject thiz) {
    XPlayer *player = getXPlayer(env, thiz);
    if (player == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    player->reset();
}

static jboolean native_isPlaying(JNIEnv *env, jobject thiz) {
    XPlayer *player = getXPlayer(env, thiz);
    if (player == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return false;
    }
    return player->isPlaying();
}

static jboolean native_isPaused(JNIEnv *env, jobject thiz) {
    XPlayer *player = getXPlayer(env, thiz);
    if (player == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return false;
    }
    return player->isPaused();
}

static JNINativeMethod gMethods[] = {
        {"native_init",           "()V",                                     (void *) native_init},
        {"native_setup",          "(Ljava/lang/Object;Ljava/lang/Object;)V", (void *) native_setup},
        {"native_release",        "()V",                                     (void *) native_release},
        {"native_finalize",       "()V",                                     (void *) native_finalize},
        {"native_setDataSource",  "(Ljava/lang/String;)V",                   (void *) native_setDataSource},
        {"native_setSurface",     "(Landroid/view/Surface;)V",               (void *) native_setSurface},
        {"native_setDisplaySize", "(II)V",                                   (void *) native_setDisplaySize},
        {"native_setScaleType",   "(I)V",                                    (void *) native_setScaleType},
        {"native_start",          "()V",                                     (void *) native_start},
        {"native_stop",           "()V",                                     (void *) native_stop},
        {"native_pause",          "()V",                                     (void *) native_pause},
        {"native_resume",         "()V",                                     (void *) native_resume},
        {"native_prepare",        "()V",                                     (void *) native_prepare},
        {"native_reset",          "()V",                                     (void *) native_reset},
        {"native_isPlaying",      "()Z",                                     (void *) native_isPlaying},
        {"native_isPaused",       "()Z",                                     (void *) native_isPaused},
};

int register_com_pujh_xplayer_XPlayer(JNIEnv *env) {
    jclass clazz = env->FindClass(ClassName);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, NELEM(gMethods)) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

JNIEXPORT jint
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOG_E("ERROR: GetEnv failed\n");
        return -1;
    }
    LOG_D("JNI_OnLoad");

    if (register_com_pujh_xplayer_XPlayer(env) != JNI_TRUE) {
        LOG_E("ERROR: MediaSource native register failed");
        return -1;
    }

    FFDecode::Init(vm);

    return JNI_VERSION_1_4;
}

//
// Created by Administrator on 2024-11-05.
//

#include "SLAudioPlayer.h"
#include "util/XLog.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

static SLObjectItf engineObj = NULL;
static SLEngineItf engineItf = NULL;

static SLObjectItf outputMixObj = NULL;

static SLObjectItf playerObj = NULL;
static SLPlayItf playerItf = NULL;
static SLAndroidSimpleBufferQueueItf bufferQueueItf = NULL;
static SLVolumeItf volumeItf = NULL;

void SLAudioPlayer::PlayCall(void *bufq) {
    if (!bufq) {
        return;
    }
    SLAndroidSimpleBufferQueueItf bf = (SLAndroidSimpleBufferQueueItf) bufq;
//    XLOGI("SLAudioPlayer::PlayCall");
    //阻塞
    XData d = GetData();
    if (d.size <= 0) {
        XLOGE("GetData() size is 0");
        return;
    }
    if (!buf) {
        return;
    }
    memcpy(buf, d.data, d.size);
    (*bf)->Enqueue(bufferQueueItf, buf, d.size);
    d.Drop();
}

static void pcmBufferCallback(SLAndroidSimpleBufferQueueItf caller, void *pContext) {
    SLAudioPlayer *ap = (SLAudioPlayer *) pContext;
    if (!ap) {
        XLOGE("pcmBufferCallback failed context is NULL!");
        return;
    }
    ap->PlayCall((void *) caller);
}

bool SLAudioPlayer::StartPlay(XParameter out) {
    SLresult re;
    //1、创建引擎
    //创建引擎
    re = slCreateEngine(&engineObj, 0, 0, 0, 0, 0);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("slCreateEngine filed!");
        return false;
    }
    //实例化
    re = (*engineObj)->Realize(engineObj, SL_BOOLEAN_FALSE);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("engineObj Realize filed!");
        return false;
    }
    //获取接口
    re = (*engineObj)->GetInterface(engineObj, SL_IID_ENGINE, &engineItf);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("engineObj GetInterface filed!");
        return false;
    }

    //2、创建混音器
    re = (*engineItf)->CreateOutputMix(engineItf, &outputMixObj, 0, 0, 0);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("CreateOutputMix filed!");
        return false;
    }
    re = (*outputMixObj)->Realize(outputMixObj, SL_BOOLEAN_FALSE);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("outputMixObj Realize filed!");
        return false;
    }

    //缓冲队列
    SLDataLocator_AndroidSimpleBufferQueue bufferQueue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
                                                          2};
    //音频格式
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            out.channels,//    声道数
            out.sample_rate * 1000,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN //字节序，小端
    };
    SLDataSource slDataSource = {&bufferQueue, &pcm};

    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObj};
    SLDataSink audioSink = {&outputMix, NULL};

    const SLInterfaceID ids[2] = {SL_IID_BUFFERQUEUE, SL_IID_VOLUME};
    const SLboolean req[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    re = (*engineItf)->CreateAudioPlayer(engineItf, &playerObj, &slDataSource, &audioSink,
                                         sizeof(ids) / sizeof(SLInterfaceID), ids, req);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("CreateAudioPlayer filed!");
        return false;
    }
    re = (*playerObj)->Realize(playerObj, SL_BOOLEAN_FALSE);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("playerObj Realize filed!");
        return false;
    }

    //获取player接口
    re = (*playerObj)->GetInterface(playerObj, SL_IID_PLAY, &playerItf);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("playerObj GetInterface SL_IID_PLAY filed!");
        return false;
    }
    re = (*playerObj)->GetInterface(playerObj, SL_IID_BUFFERQUEUE, &bufferQueueItf);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("playerObj GetInterface SL_IID_BUFFERQUEUE filed!");
        return false;
    }
    re = (*playerObj)->GetInterface(playerObj, SL_IID_VOLUME, &volumeItf);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("playerObj GetInterface SL_IID_VOLUME filed!");
        return false;
    }
    //设置回调函数，播放队列空调用
    re = (*bufferQueueItf)->RegisterCallback(bufferQueueItf, pcmBufferCallback, this);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("RegisterCallback filed!");
        return false;
    }

    //设置为播放状态
    re = (*playerItf)->SetPlayState(playerItf, SL_PLAYSTATE_PLAYING);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("SetPlayState filed!");
        return false;
    }
    //启动队列回调
    re = (*bufferQueueItf)->Enqueue(bufferQueueItf, "", 1);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("Enqueue filed!");
        return false;
    }
    XLOGI("SLAudioPlayer::StartPlay success!");
    return true;
}

SLAudioPlayer::SLAudioPlayer() {
    buf = new unsigned char[1024 * 1024];
}

SLAudioPlayer::~SLAudioPlayer() {
    delete buf;
}


//
// Created by 24415 on 2024-11-05.
//

#include "FFResample.h"
#include "util/XLog.h"

extern "C" {
#include "libswresample/swresample.h"
#include "libavcodec/avcodec.h"
}

bool FFResample::Open(XParameter in, XParameter out) {
    //音频重采样上下文初始化
    actx = swr_alloc();
    actx = swr_alloc_set_opts(actx,
                              av_get_default_channel_layout(2),
                              AV_SAMPLE_FMT_S16,
                              out.sample_rate,
                              in.para->channel_layout,
                              (AVSampleFormat) in.para->format,
                              in.para->sample_rate,
                              0, NULL);
    int ret = swr_init(actx);
    if (ret != 0) {
        XLOGE("swr_init failed!");
        return false;
    } else {
        XLOGI("swr_init success!");
    }
    outChannels = 2;
    outFormat = AV_SAMPLE_FMT_S16;
    return true;
}

XData FFResample::Resample(XData inData) {
//    int len = swr_convert(actx,
//                          out, frame->nb_samples,
//                          (const uint8_t **) frame->data, frame->nb_samples);
    if (!actx) {
        return XData();
    }
    if (inData.size <= 0 || !inData.data) {
        return XData();
    }
    AVFrame *frame = (AVFrame *) inData.data;

    //输出空间分配
    XData out;
    //通道数 * 单通道样本数 * 样本字节大小
    int outSize = outChannels * frame->nb_samples
                  * av_get_bytes_per_sample((AVSampleFormat) outFormat);
    if (outSize <= 0) {
        return XData();
    }
    out.Alloc(outSize);
    uint8_t *outArr[2] = {0};
    outArr[0] = out.data;
    int len = swr_convert(actx, outArr, frame->nb_samples, (const uint8_t **) frame->data,
                          frame->nb_samples);
    if (len <= 0) {
        out.Drop();
        return XData();
    }
//    XLOGD("swr_convert success = %d", len);
    return out;
}

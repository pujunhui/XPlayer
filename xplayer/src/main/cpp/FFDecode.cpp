//
// Created by 24415 on 2024-11-04.
//

#include "FFDecode.h"
#include "util/XLog.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavcodec/jni.h"
#include "libavutil/pixdesc.h"
}

bool FFDecode::Open(XParameter para, bool hard) {
    if (!para.para) return false;
    AVCodecParameters *p = para.para;
    //1 查找解码器
    AVCodec *cd;
    if (hard) {
        cd = avcodec_find_decoder_by_name("h264_mediacodec");
    } else {
        cd = avcodec_find_decoder(p->codec_id);
    }
    if (!cd) {
        XLOGE("avcodec_find_decoder %d failed!", p->codec_id);
        return false;
    }
    XLOGI("avcodec_find_decoder success!");

    //2 创建解码上下文，并复制参数
    codec = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(codec, p);
    codec->thread_count = 8; //设置8线程解码

    //3 打开解码器
    int re = avcodec_open2(codec, 0, 0);
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf) - 1);
        XLOGE("%s", buf);
        return false;
    }

    if (codec->codec_type == AVMEDIA_TYPE_VIDEO) {
        this->isAudio = false;
        XLOGD("Video Stream:\n");
        XLOGD("  Width: %d\n", codec->width);
        XLOGD("  Height: %d\n", codec->height);
        XLOGD("  Pixel Format: %s\n", av_get_pix_fmt_name((AVPixelFormat) codec->pix_fmt));
        XLOGD("  Codec: %d\n", codec->codec_id);
        XLOGD("  Hard decode: %d\n", hard);
    } else {
        this->isAudio = true;
        XLOGD("Audio Stream:\n");
        XLOGD("  Codec: %d\n", codec->codec_id);
        XLOGD("  Hard decode: %d\n", hard);
    }

    XLOGI("avcodec_open2 success!");
    return true;
}

bool FFDecode::SendPacket(XData pkt) {
    if (pkt.size <= 0 || !pkt.data) {
        return false;
    }
    if (!codec) {
        return false;
    }
    int re = avcodec_send_packet(codec, (AVPacket *) pkt.data);
    if (re != 0) {
        return false;
    }
    return true;
}

//从线程中获取解码结果
XData FFDecode::RecvFrame() {
    if (!codec) {
        return XData();
    }
    if (!frame) {
        frame = av_frame_alloc();
    }
    int re = avcodec_receive_frame(codec, frame);
    if (re != 0) {
        return XData();
    }
    XData d;
    d.data = (unsigned char *) frame;
    if (codec->codec_type == AVMEDIA_TYPE_VIDEO) {
        d.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2]) * frame->height;
        d.width = frame->width;
        d.height = frame->height;
        d.format = frame->format;
    } else {
        //样本字节数 * 单通道样本数 * 通道数
        d.size = av_get_bytes_per_sample((AVSampleFormat) frame->format) * frame->nb_samples * 2;
        d.format = frame->format;
    }
    memcpy(d.datas, frame->data, sizeof(d.datas));
    return d;
}

void FFDecode::Init(JavaVM *vm) {
    av_jni_set_java_vm(vm, NULL);
}

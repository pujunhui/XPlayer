//
// Created by 24415 on 2024-11-04.
//

#ifndef FFMPEG_FFDECODE_H
#define FFMPEG_FFDECODE_H

#include <jni.h>
#include "IDecode.h"

struct AVCodecContext;
struct AVFrame;

class FFDecode : public IDecode {
public:

    static void Init(JavaVM *vm);

    bool Open(XParameter para, bool hard = false) override;

    //future模型
    //发送数据到线程解码
    bool SendPacket(XData pkt) override;

    //从线程中获取解码结果，再次调用会复用上次空间，线程不安全
    XData RecvFrame() override;

protected:
    AVCodecContext *codec = NULL;
    AVFrame *frame = NULL;
};


#endif //FFMPEG_FFDECODE_H

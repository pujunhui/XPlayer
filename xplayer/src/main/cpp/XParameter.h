//
// Created by 24415 on 2024-11-04.
//

#ifndef FFMPEG_XPARAMETER_H
#define FFMPEG_XPARAMETER_H

struct AVCodecParameters;

class XParameter {
public:
    AVCodecParameters *para = 0;
    unsigned int channels = 2;
    unsigned int sample_rate = 44100;
};


#endif //FFMPEG_XPARAMETER_H

//
// Created by 24415 on 2024-11-13.
//

#ifndef FFMPEG_YUV420PRENDER_H
#define FFMPEG_YUV420PRENDER_H


#include "../XGLRender.h"

class YUV420PRender : public XGLRender {
public:
    bool InitTexture(int width, int height) override;

    int getFragShader(char *fShader, int size) override;

    int Render(unsigned char *data[], int width, int height) override;

private:
    GLuint texts[3] = {0};
};


#endif //FFMPEG_YUV420PRENDER_H

//
// Created by 24415 on 2024-11-13.
//

#ifndef FFMPEG_NV12RENDER_H
#define FFMPEG_NV12RENDER_H


#include "../XGLRender.h"

class NV12Render : public XGLRender {
public:
    bool InitTexture(int width, int height) override;

    int getFragShader(char *fShader, int size) override;

    int Render(unsigned char *data[], int width, int height) override;

private:
    GLuint texts[2] = {0};
};


#endif //FFMPEG_NV12RENDER_H

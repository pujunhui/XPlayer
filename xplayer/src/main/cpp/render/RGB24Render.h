//
// Created by 24415 on 2024-11-13.
//

#ifndef FFMPEG_RGB24RENDER_H
#define FFMPEG_RGB24RENDER_H


#include "../XGLRender.h"

class RGB24Render : public XGLRender {
public:
    bool InitTexture(int width, int height) override;

    int getFragShader(char *fShader, int size) override;

    int Render(unsigned char *data[], int width, int height) override;
};


#endif //FFMPEG_RGB24RENDER_H

//
// Created by 24415 on 2024-11-13.
//

#ifndef FFMPEG_XGLRENDER_H
#define FFMPEG_XGLRENDER_H


#include <android/asset_manager.h>
#include <GLES3/gl3.h>

class XGLRender {
public:
    static void Init(AAssetManager *assets);

    static XGLRender *getRender(int format);

    virtual bool Init() final;

    virtual int Render(unsigned char *data[], int width, int height) = 0;

private:
    virtual int getVertShader(char *vShader, int size) final;

protected:
    virtual bool InitTexture(int width, int height) = 0;

    virtual int getFragShader(char *fShader, int size) = 0;

protected:
    GLuint vao = 0;
    GLuint program = 0;
    static AAssetManager *mAssets;
};


#endif //FFMPEG_XGLRENDER_H

//
// Created by 24415 on 2024-11-13.
//

#include "XGLRender.h"

#include <GLES3/gl3.h>
#include "utils/GLUtil.h"
#include "util/XLog.h"

#include "utils/AAssetUtil.h"

#include "render/YUV420PRender.h"
#include "render/RGB24Render.h"
#include "render/NV12Render.h"
#include "render/NV21Render.h"

extern "C" {
#include "libavutil/pixfmt.h"
}

static float vertices[] = {
        //         顶点坐标                纹理坐标
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,       //右下角
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,      //左下角
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,    //右上角
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,   //左上角
};
static unsigned indices[] = {
        0, 1, 2, //第一个三角形
        1, 2, 3  //第二个三角形
};

AAssetManager *XGLRender::mAssets = NULL;

void XGLRender::Init(AAssetManager *assets) {
    mAssets = assets;
}

XGLRender *XGLRender::getRender(int format) {
    if (format == AV_PIX_FMT_YUV420P) {
        return new YUV420PRender();
    } else if (format == AV_PIX_FMT_NV12) {
        return new NV12Render();
    } else if (format == AV_PIX_FMT_NV21) {
        return new NV21Render();
    } else if (format == AV_PIX_FMT_RGB24) {
        return new RGB24Render();
    }
    return NULL;
}

bool XGLRender::Init() {
    char vShaderSource[1024];
    char fShaderSource[1024];
    getVertShader(vShaderSource, 1024);
    getFragShader(fShaderSource, 1024);

    program = createProgram(vShaderSource, fShaderSource);

    if (program == 0) {
        XLOGE("createProgram failed!");
        return false;
    }
    //使用program
    glUseProgram(program);

    GLuint aPosition = glGetAttribLocation(program, "aPosition");
    GLuint aTexCoord = glGetAttribLocation(program, "aTexCoord");

    glUseProgram(0);

    //创建vao,用于保存所有的状态信息，以便在绘制时可以方便地绑定
//    GLuint vao;
    glGenVertexArrays(1, &vao);

    //创建vbo,用于存储顶点数据
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //创建ebo,用于存储索引数据，以便在绘制时重用顶点。
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glEnableVertexAttribArray(aPosition);
    glVertexAttribPointer(aPosition, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);

    glEnableVertexAttribArray(aTexCoord);
    glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *) (3 * sizeof(float)));

    glBindVertexArray(0);

    return InitTexture(1920, 1080);
}


int XGLRender::getVertShader(char *vShader, int size) {
    return readAssetFile(mAssets, "vertexShader.vert", vShader, size);
}


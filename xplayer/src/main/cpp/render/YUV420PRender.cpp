//
// Created by 24415 on 2024-11-13.
//

#include "YUV420PRender.h"

#include "../utils/AAssetUtil.h"
#include "../utils/GLUtil.h"
#include "../util/XLog.h"
#include <malloc.h>


bool YUV420PRender::InitTexture(int width, int height) {
    //使用program
    glUseProgram(program);

    GLint yTexture = glGetUniformLocation(program, "yTexture");
    GLint uTexture = glGetUniformLocation(program, "uTexture");
    GLint vTexture = glGetUniformLocation(program, "vTexture");
    glUniform1i(yTexture, 0);
    glUniform1i(uTexture, 1);
    glUniform1i(vTexture, 2);

    glUseProgram(0);

    glGenTextures(3, texts);

    glBindTexture(GL_TEXTURE_2D, texts[0]);
    //设置环绕和过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //水平环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //垂直环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //下采样过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //上采样过滤方式
    glTexImage2D(GL_TEXTURE_2D,
                 0,                           //细节基本 0默认
                 GL_LUMINANCE,         //gpu内部格式 亮度，灰度图
                 width, height,                    //拉升到全屏
                 0,                         //边框
                 GL_LUMINANCE,              //数据的像素格式 亮度，灰度图 要与上面一致
                 GL_UNSIGNED_BYTE,            //像素的数据类型
                 NULL                        //纹理的数据
    );

    glBindTexture(GL_TEXTURE_2D, texts[1]);
    //设置环绕和过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //水平环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //垂直环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //下采样过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //上采样过滤方式
    glTexImage2D(GL_TEXTURE_2D,
                 0,                           //细节基本 0默认
                 GL_LUMINANCE,         //gpu内部格式 亮度，灰度图
                 width / 2, height / 2,//拉升到全屏
                 0,                         //边框
                 GL_LUMINANCE,              //数据的像素格式 亮度，灰度图 要与上面一致
                 GL_UNSIGNED_BYTE,            //像素的数据类型
                 NULL                        //纹理的数据
    );

    glBindTexture(GL_TEXTURE_2D, texts[2]);
    //设置环绕和过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //水平环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //垂直环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //下采样过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //上采样过滤方式
    glTexImage2D(GL_TEXTURE_2D,
                 0,                           //细节基本 0默认
                 GL_LUMINANCE,         //gpu内部格式 亮度，灰度图
                 width / 2, height / 2,//拉升到全屏
                 0,                         //边框
                 GL_LUMINANCE,              //数据的像素格式 亮度，灰度图 要与上面一致
                 GL_UNSIGNED_BYTE,            //像素的数据类型
                 NULL                        //纹理的数据
    );

    return true;
}

int YUV420PRender::getFragShader(char *fShader, int size) {
    return readAssetFile(mAssets, "fragYUV420P.frag", fShader, size);
}

int YUV420PRender::Render(unsigned char **data, int width, int height) {
    glUseProgram(program);

    //激活第0层纹理,绑定到创建的opengl纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texts[0]);
    //替换纹理内容
    glTexSubImage2D(GL_TEXTURE_2D, 0,
                    0, 0, width, height,
                    GL_LUMINANCE, GL_UNSIGNED_BYTE,
                    data[0]);


    //激活第1层纹理,绑定到创建的opengl纹理
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texts[1]);
    //替换纹理内容
    glTexSubImage2D(GL_TEXTURE_2D, 0,
                    0, 0, width / 2, height / 2,
                    GL_LUMINANCE, GL_UNSIGNED_BYTE,
                    data[1]);

    //激活第2层纹理,绑定到创建的opengl纹理
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texts[2]);
    //替换纹理内容
    glTexSubImage2D(GL_TEXTURE_2D, 0,
                    0, 0, width / 2, height / 2,
                    GL_LUMINANCE, GL_UNSIGNED_BYTE,
                    data[2]);

    glBindVertexArray(vao);

    //三维绘制
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
    return 0;
}

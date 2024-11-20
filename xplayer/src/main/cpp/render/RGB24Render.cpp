//
// Created by 24415 on 2024-11-13.
//

#include "RGB24Render.h"

#include "../utils/AAssetUtil.h"
#include "../utils/GLUtil.h"
#include "../util/XLog.h"

bool RGB24Render::InitTexture(int width, int height) {
    return false;
}

int RGB24Render::getFragShader(char *fShader, int size) {
    return readAssetFile(mAssets, "fragRGB24.frag", fShader, size);
}

int RGB24Render::Render(unsigned char **data, int width, int height) {
    return 0;
}

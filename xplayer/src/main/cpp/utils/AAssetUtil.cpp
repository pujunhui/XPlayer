//
// Created by iDste-PC on 2024-07-30.
//

#include "AAssetUtil.h"
#include <android/asset_manager_jni.h>
#include <malloc.h>
#include "log.h"

int readAssetFile(AAssetManager *pManager, const char *filename, char **output) {
    AAsset *pAsset = AAssetManager_open(pManager, filename, AASSET_MODE_BUFFER);
    size_t count = AAsset_getLength(pAsset);
    char *buf = (char *) malloc(count + 1);
    if (buf) {
        AAsset_read(pAsset, buf, count);
        buf[count] = '\0';
//        LOG_I("read asset file: %s\n%s", filename, buf);
        *output = buf;
    } else {
        count = -1;
        *output = NULL;
        free(buf);
        LOG_E("read asset file filed: %s", filename);
    }
    AAsset_close(pAsset);
    return count;
}

int readAssetFile(AAssetManager *pManager, const char *filename, char *output, int size) {
    AAsset *pAsset = AAssetManager_open(pManager, filename, AASSET_MODE_BUFFER);
    size_t count = AAsset_getLength(pAsset);
    if (size >= count + 1) {
        AAsset_read(pAsset, output, count);
        output[count] = '\0';
        LOG_I("read asset file: %s\n%s", filename, output);
    } else {
        count = -1;
        LOG_E("read asset file filed: %s", filename);
    }
    AAsset_close(pAsset);
    return count;
}
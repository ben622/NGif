//
// Created by 599 on 2020/7/3.
//
#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <android/bitmap.h>
#include "log.h"
#include "gif_lib.h"
#include <malloc.h>

typedef struct Gif {
    int current_frame;
    int total_frame;
    int *delays;
};

static jlong
nativeInitlizate(JNIEnv *env, jclass c, jstring jpath) {
    char *path = const_cast<char *>(env->GetStringUTFChars(jpath, JNI_FALSE));
    int error;
    GifFileType *gifFileType =  DGifOpenFileName(path, &error);
    if (error) {
        LOGE("open file %s failed code[%d]", path,error);
        return 0;
    }
    //init gif.
    DGifSlurp(gifFileType);


    Gif *gif = new Gif();
    gif->current_frame = 0;
    gif->total_frame = gifFileType->ImageCount;
    gif->delays = static_cast<int *>(malloc(sizeof(int) * gifFileType->ImageCount));
    memset(gif->delays, 0, sizeof(int) * gifFileType->ImageCount);

    //calc delay.
    for (int j = 0; j < gif->total_frame; ++j) {
        SavedImage savedImage = gifFileType->SavedImages[j];
        ExtensionBlock *extensionBlock;
        //在每帧中查找图像扩展块
        for (int i = 0; i < savedImage.ExtensionBlockCount; ++i) {
            if (savedImage.ExtensionBlocks[i].Function == GRAPHICS_EXT_FUNC_CODE) {
                //图像扩展块
                extensionBlock = &savedImage.ExtensionBlocks[i];
                break;
            }
        }
        if (extensionBlock) {
            //两字节中高低位运算 10ms为单位
            int delay = (extensionBlock->Bytes[2] << 8 | extensionBlock->Bytes[1]) * 10;
            gif->delays[j] = delay;
        }

    }
    //附加结构
    gifFileType->UserData = gif;

    env->ReleaseStringChars(jpath, reinterpret_cast<const jchar *>(path));
    return (jlong) gifFileType;
}

static jint
nativeGetWidth(JNIEnv *env, jclass c, jlong ptr) {
    if (ptr <= 0) {
        LOGE("%s", "input ptr error!");
        return 0;
    }
    GifFileType *gifFileType = reinterpret_cast<GifFileType *>(ptr);
    return gifFileType->SWidth;
}

static jint
nativeGetHeight(JNIEnv *env, jclass c, jlong ptr) {
    if (ptr <= 0) {
        LOGE("%s", "input ptr error!");
        return 0;
    }
    GifFileType *gifFileType = reinterpret_cast<GifFileType *>(ptr);
    return gifFileType->SHeight;
}

static jint
nativeRender(JNIEnv *env, jclass c, jlong ptr, jobject jbitmap) {
    if (ptr <= 0) {
        LOGE("%s", "input ptr error!");
        return -1;
    }

    GifFileType *gifFileType = reinterpret_cast<GifFileType *>(ptr);
    Gif *gif = static_cast<Gif *>(gifFileType->UserData);

    //get bitmap pixel.
    AndroidBitmapInfo bitmapInfo;
    int result = AndroidBitmap_getInfo(env, jbitmap, &bitmapInfo);
    if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
        if (LOGS_ENABLED) LOGE("%s", "get bitmap info failed");
        return -1;
    }
    void *pixel;
    result = AndroidBitmap_lockPixels(env, jbitmap, &pixel);
    if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
        if (LOGS_ENABLED) LOGE("%s", "lock bitmap pixels failed");
        return -1;
    }


    //draw
    SavedImage savedImage = gifFileType->SavedImages[gif->current_frame];
    GifImageDesc gifImageDesc = savedImage.ImageDesc;

    ColorMapObject *colorMapObject = gifFileType->SColorMap;
    //内容距离偏移
    int *px = static_cast<int *>(pixel);
    px = (int *) ((char*)px + bitmapInfo.stride * gifImageDesc.Top);
    int *line;
    GifByteType gifByteType;
    GifColorType gifColorType;
    for (int y = gifImageDesc.Top; y < gifImageDesc.Top + gifImageDesc.Height; ++y) {
        line = px;
        for (int x = gifImageDesc.Left; x < gifImageDesc.Left + gifImageDesc.Width; ++x) {
            int baster = (y - gifImageDesc.Top) * gifImageDesc.Width + /**字节对齐*/(x - gifImageDesc.Left);
            gifByteType = savedImage.RasterBits[baster];
            gifColorType = colorMapObject->Colors[gifByteType];
            line[x] = (((255) & 0xff) << 24) | ((gifColorType.Red) & 0xff)| (((gifColorType.Green) & 0xff) << 8)| (((gifColorType.Blue) & 0xff) << 16);
        }
        px = (int *) ((char*)px + bitmapInfo.stride);
    }

    gif->current_frame += 1;
    if (gif->current_frame >= gif->total_frame - 1) {
        gif->current_frame = 0;
    }
    if (LOGS_ENABLED) LOGI("tootal frame:%d render frame:%d", gif->total_frame,gif->current_frame);

    AndroidBitmap_unlockPixels(env, jbitmap);

    return gif->delays[gif->current_frame];
}




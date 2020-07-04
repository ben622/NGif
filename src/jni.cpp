//
// Created by ben622 on 2020/1/6.
//
#include <stdio.h>
#include <string.h>
#include <jni.h>
#include "log.h"
#include "gif_loader.cpp"

static const char *JavaGifLoaderClassName = "com/ben/android/ngif/GifLoader";
static JNINativeMethod GifLoaderMethods[] = {
        {"nativeInitlizate", "(Ljava/lang/String;)J",                            (void *) nativeInitlizate},
        {"nativeGetWidth",   "(J)I",                                             (void *) nativeGetWidth},
        {"nativeGetHeight",  "(J)I",                                             (void *) nativeGetHeight},
        {"nativeRender",     "(JLandroid/graphics/Bitmap;)I", (void *) nativeRender}
};

int registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *methods,
                          int methodsCount) {
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, methods, methodsCount) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = 0;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    if (!registerNativeMethods(env, JavaGifLoaderClassName, GifLoaderMethods,
                               sizeof(GifLoaderMethods) /
                               sizeof(GifLoaderMethods[0]))) {
        return JNI_FALSE;
    }

    return JNI_VERSION_1_6;
}
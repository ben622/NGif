MY_LOCAL_PATH := $(call my-dir)
LOCAL_PATH := $(MY_LOCAL_PATH)

include $(CLEAR_VARS)
LOCAL_MODULE 	:= ngif
LOCAL_CPPFLAGS 	:= -std=c++11
LOCAL_LDLIBS 	:= -llog -ljnigraphics -landroid
LOCAL_SRC_FILES     := jni.cpp gif_loader.cpp dgif_lib.c egif_lib.c gif_err.c gif_font.c gif_hash.c gifalloc.c quantize.c openbsd-reallocarray.c
include $(BUILD_SHARED_LIBRARY)
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

#OPENCV_LIB_TYPE:=STATIC
OPENCV_CAMERA_MODULES:=off
OPENCV_INSTALL_MODULES:=on

include /Users/view/Libraries/OpenCV-2.4.7.1-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_C_INCLUDES += /Users/view/Libraries/OpenCV-2.4.7.1-android-sdk/sdk/native/jni/include/

LOCAL_CFLAGS += -g -D__STDC_CONSTANT_MACROS -DANDROID

LOCAL_LDLIBS += -llog -lGLESv2

LOCAL_MODULE  := rectangledetector

LOCAL_SRC_FILES := \
 	RectangleDetector.cpp \
 	RectangleFinder.cpp \
 	CameraRenderer.cpp \
	CameraUtil.cpp \
	yuv-neon.s

LOCAL_SHARED_LIBRARIES += liblog libGLESv2

include $(BUILD_SHARED_LIBRARY)

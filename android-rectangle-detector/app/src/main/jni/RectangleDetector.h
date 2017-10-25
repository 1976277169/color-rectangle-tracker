#ifndef __RectangleDetector__
#define __RectangleDetector__

#include <jni.h>
#include <pthread.h>

#include "opencv2/opencv.hpp"
#include "ARRenderer.h"
#include "CameraRenderer.h"
#include "RectangleFinder.h"

#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480

extern "C" {


    JNIEXPORT void JNICALL Java_org_astri_rectangledetector_MainActivity_startProcessing
	(
     JNIEnv* env,
     jobject obj
    );

    JNIEXPORT void JNICALL Java_org_astri_rectangledetector_MainActivity_stopProcessing
	(
     JNIEnv* env,
     jobject obj
    );

    JNIEXPORT void JNICALL Java_org_astri_rectangledetector_MainActivity_setCameraFrame
    (
     JNIEnv*     env,
     jobject     obj,
     jbyteArray  img
    );


    void AR_Init(int camera_width, int camera_height);
    void AR_Process();

    char appDataDir[255];

    unsigned char* renderBufRGB565;
	uint32_t renderBufWidth;
	uint32_t renderBufHeight;
	unsigned char* greyscaleBuffer;

    pthread_mutex_t renderMutex;

    pthread_t processingThread;
    static bool isProcessing;

    CameraRenderer* cameraRenderer;
    RectangleFinder rectFinder;

}

#endif

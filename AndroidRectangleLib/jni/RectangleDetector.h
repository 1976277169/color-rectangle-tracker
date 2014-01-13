#ifndef __RectangleDetector__
#define __RectangleDetector__

#include <jni.h>
#include <pthread.h>

#include "opencv2/opencv.hpp"
#include "UnityCameraRenderer.h"
#include "../../RectangleFinder/RectangleFinder.h"

#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480
#define EXPORT_API

extern "C" {


    JNIEXPORT void JNICALL Java_org_astri_unityrect_RectangleDetector_setCameraFrame
    (
     JNIEnv*     env,
     jobject     obj,
     jbyteArray  img
    );

    void EXPORT_API AR_InitBackgroundRender(int textureId);
    void EXPORT_API AR_RenderBackgroundFrame();
    int  EXPORT_API AR_GetMarkerLocation(int* array);

}

    char appDataDir[255];

    unsigned char* renderBufRGB565;
	uint32_t renderBufWidth;
	uint32_t renderBufHeight;
	unsigned char* greyscaleBuffer;

    pthread_mutex_t renderMutex;

    pthread_t processingThread;
    static bool isProcessing;

    UnityCameraRenderer cameraRenderer;
    RectangleFinder rectFinder;
    int rectangle[5];

#endif

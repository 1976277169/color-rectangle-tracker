#include "RectangleDetector.h"
#include "YUV420toRGB565.h"
#include "../../RectangleFinder/Logwrapper.h"

#include <sys/time.h>
#include <time.h>


void unlockRenderBuffer() {
	pthread_mutex_unlock(&(renderMutex));
}

void lockRenderBuffer() {
	int rc = pthread_mutex_lock(&(renderMutex));
	if (rc != 0) {
		LOGE( "Error locking mutex: %d\n", rc);
	}
}

cv::Mat camImgTmp = cv::Mat::zeros(480, 640, CV_8UC2);
cv::Mat tmpImgRGB = cv::Mat::zeros(480, 640, CV_8UC3);

void processFrame(){

	camImgTmp.data = renderBufRGB565;
	if(camImgTmp.empty()){
		LOGD("Empty frame!");
		return;
	}
	cv::cvtColor(camImgTmp, tmpImgRGB, CV_BGR5652BGR);
	rectFinder.detectRectangles(camImgTmp, rectangle);

}

JNIEXPORT void JNICALL Java_org_astri_unityrect_RectangleDetector_setCameraFrame
(
 JNIEnv*     env,
 jobject     obj,
 jbyteArray  img
)
{

    jbyte* jimgData = NULL;
    jboolean isCopy = 0;

    // Get data from JNI
    jimgData = env->GetByteArrayElements( img, &isCopy );

    lockRenderBuffer();

    uint8_t* pJimgData = (uint8_t*)jimgData;

    toRGB565neon((unsigned char *)pJimgData,
                 renderBufWidth, renderBufHeight,
                 (unsigned short *)renderBufRGB565,
                 (unsigned char *)greyscaleBuffer);

    processFrame();

    unlockRenderBuffer();

    // Let JNI know we don't need data anymore. this is important!
    env->ReleaseByteArrayElements( img, jimgData, JNI_ABORT );


}

void AR_InitBackgroundRender(int textureId){

    int renderBufSize = FRAME_WIDTH * FRAME_HEIGHT * 4;
	renderBufRGB565 = (uint8_t*) malloc(renderBufSize);
	renderBufWidth = FRAME_WIDTH;
	renderBufHeight = FRAME_HEIGHT;

	greyscaleBuffer = (uint8_t*) malloc(renderBufSize);

	cameraRenderer.initRendering(textureId, FRAME_WIDTH, FRAME_HEIGHT);

}

void AR_InitColorRange(int minB, int maxB, int minG, int maxG, int minR, int maxR){

	rectFinder.setColorRange(minB, maxB, minG, maxG, minR, maxR);

}

void AR_RenderBackgroundFrame(){

    lockRenderBuffer();
    cameraRenderer.renderCameraFrame(renderBufRGB565);
    unlockRenderBuffer();
}

int AR_GetMarkerLocation(int* array){

	array[0] = rectangle[0];
	array[1] = rectangle[1];
	array[2] = rectangle[2];
	array[3] = rectangle[3];
	array[4] = rectangle[4];

	return rectangle[0];
}


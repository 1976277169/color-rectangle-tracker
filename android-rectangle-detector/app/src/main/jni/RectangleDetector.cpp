#include "RectangleDetector.h"
#include "YUV420toRGB565.h"
#include "Logwrapper.h"

#include <sys/time.h>
#include <time.h>

uint64_t    current_time;

uint64_t    GetTime()
{
    struct  timeval     _tv;
    struct  timezone    _tz;
    gettimeofday(&_tv, &_tz);
    return _tv.tv_sec * 1000000 + _tv.tv_usec;
}


void unlockRenderBuffer() {
	pthread_mutex_unlock(&(renderMutex));
}

void lockRenderBuffer() {
	int rc = pthread_mutex_lock(&(renderMutex));
	if (rc != 0) {
		LOGE( "Error locking mutex: %d\n", rc);
	}
}


JNIEXPORT void JNICALL Java_org_astri_rectangledetector_MainActivity_startProcessing
(
	JNIEnv* env,
	jobject obj
)
{
	AR_Init(FRAME_WIDTH, FRAME_HEIGHT);
    AR_Process();
}

JNIEXPORT void JNICALL Java_org_astri_rectangledetector_MainActivity_stopProcessing
(
 JNIEnv* env,
 jobject obj
 )
{
    isProcessing = false;
}

cv::Mat camImgTmp = cv::Mat::zeros(480, 640, CV_8UC2);
int rectArray[5];

void processFrame(){

	camImgTmp.data = renderBufRGB565;
	rectFinder.detectRectangles(camImgTmp, rectArray);

}

void* processLoop(void* ptr){
	return 0;
}

void AR_Process(){
    isProcessing = true;
    //pthread_create(&processingThread, NULL, processLoop, NULL);

}

void AR_Init(int camera_width, int camera_height){

    int renderBufSize = camera_width * camera_height * 4;
	renderBufRGB565 = (uint8_t*) malloc(renderBufSize);
	renderBufWidth = camera_width;
	renderBufHeight = camera_height;

	greyscaleBuffer = (uint8_t*) malloc(renderBufSize);

}

JNIEXPORT void JNICALL Java_org_astri_rectangledetector_MainActivity_setCameraFrame
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



JNIEXPORT void JNICALL Java_org_astri_rendering_ARRenderer_initRendering
(
 JNIEnv*  env,
 jobject  obj
 )
{
	if(!cameraRenderer){
		cameraRenderer = new CameraRenderer();
		cameraRenderer->Init();
	}

}

JNIEXPORT void JNICALL Java_org_astri_rendering_ARRenderer_surfaceChanged
(
 JNIEnv* env,
 jobject obj,
 jint x,
 jint y,
 jint width,
 jint height
 )
{
	// surface changed
	glViewport(x, y, width, height);
}

JNIEXPORT void JNICALL Java_org_astri_rendering_ARRenderer_render
(
 JNIEnv* env,
 jobject obj
 )
{
	lockRenderBuffer();
	cameraRenderer->Render(renderBufRGB565,
						   renderBufWidth,
						   renderBufHeight);
	unlockRenderBuffer();
	glFinish();

}


JNIEXPORT void JNICALL Java_org_astri_rendering_ARRenderer_renderObject
(
 JNIEnv* env,
 jobject obj
 )
{

}

JNIEXPORT void JNICALL Java_org_astri_rendering_ARRenderer_cleanup
(
 JNIEnv* env,
 jobject obj
 )
{
	cameraRenderer->UnInit();
}


#include "RectangleDetector.h"
#include "YUV420toRGB565.h"
#include "Logwrapper.h"

#include <sys/time.h>
#include <time.h>
#include <vector>
#include <math.h>

int thresh = 50, N = 1; // 11

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

// the function draws all the squares in the image
static void drawSquares( cv::Mat& image, const std::vector<std::vector<cv::Point> >& squares )
{
	LOGD("squares: %i", squares.size());
    for( size_t i = 0; i < squares.size(); i++ ){
        const cv::Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        cv::polylines(image, &p, &n, 1, true, cv::Scalar(0,255,0), 3, CV_AA);
    }

}

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2) / std::sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
static void findSquares( const cv::Mat& image, std::vector<std::vector<cv::Point> >& squares )
{
    squares.clear();

    cv::Mat pyr, timg, gray0(image.size(), CV_8U), gray;

    // down-scale and upscale the image to filter out the noise
    cv::pyrDown(image, pyr, cv::Size(image.cols/2, image.rows/2));
    cv::pyrUp(pyr, timg, image.size());
    std::vector<std::vector<cv::Point> > contours;

    // find squares in every color plane of the image
    for(int c = 1; c < 2; c++ )
    {
        int ch[] = {c, 0};
        cv::mixChannels(&timg, 1, &gray0, 1, ch, 1);

        // try several threshold levels
        for( int l = 0; l < N; l++ )
        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            if( l == 0 )
            {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                cv::Canny(gray0, gray, 0, thresh, 5);
                // dilate canny output to remove potential
                // holes between edge segments
                cv::dilate(gray, gray, cv::Mat(), cv::Point(-1,-1));
            }
            else
            {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                gray = gray0 >= (l+1)*255/N;
            }

            // find contours and store them all as a list
            cv::findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

            std::vector<cv::Point> approx;

            // test each contour
            for( size_t i = 0; i < contours.size(); i++ ){
                // approximate contour with accuracy proportional
                // to the contour perimeter
                cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( approx.size() == 4 &&
                    std::fabs(cv::contourArea(cv::Mat(approx))) > 1000 &&
                    cv::isContourConvex(cv::Mat(approx)) )
                {
                    double maxCosine = 0;

                    for( int j = 2; j < 5; j++ ){
                        // find the maximum cosine of the angle between joint edges
                        double cosine = std::fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = std::max(maxCosine, cosine);
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( maxCosine < 0.3 ){
                        squares.push_back(approx);
                        break;
                    }
                }
            }
        }
    }
}


cv::Mat camImgTmp = cv::Mat::zeros(480, 640, CV_8UC2);
cv::Mat tmpImgRGB = cv::Mat::zeros(480, 640, CV_8UC3);
cv::Mat filtered = cv::Mat::zeros(480, 640, CV_8U);

void processFrame(){

	camImgTmp.data = renderBufRGB565;
	cv::cvtColor(camImgTmp, tmpImgRGB, CV_BGR5652BGR);

	if( tmpImgRGB.empty() ){
		LOGD("frame is empty");
	    return;
	}
	LOGD("Process frame");

	cv::inRange(tmpImgRGB, cv::Scalar(0, 0, 90), cv::Scalar(70, 70, 255), filtered);

	//std::vector<std::vector<cv::Point> > squares;
	//findSquares(tmpImgRGB, squares);
	//drawSquares(tmpImgRGB, squares);

	cv::cvtColor(filtered, camImgTmp, CV_GRAY2BGR565);

}

void* processLoop(void* ptr){


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


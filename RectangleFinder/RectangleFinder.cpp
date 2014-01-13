#include "RectangleFinder.h"

#include <math.h>

RectangleFinder::RectangleFinder(){
	init();
}

void RectangleFinder::init(){
	tmpImgRGB = cv::Mat::zeros(480, 640, CV_8UC3);
	filtered = cv::Mat::zeros(480, 640, CV_8U);

	const int arr[] = {1,1,3,5,7,9,11,9,7,5,3,1,1};
	std::vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));
	diamond = cv::Mat(13, 13, CV_8UC1, cv::Scalar(255));
	gendiamond(diamond, vec);
}

void RectangleFinder::detectRectangles(cv::Mat& image, int* rectArray){

    //LOGD("Process frame");
    
	if( image.empty() ){
		LOGD("frame is empty");
		return;
	}
    
    cv::cvtColor(image, tmpImgRGB, CV_BGR5652BGR);

	cv::inRange(tmpImgRGB, cv::Scalar(0, 0, 90), cv::Scalar(60, 60, 255), filtered);

	std::vector<std::vector<cv::Point> > squares;
	std::vector<cv::Rect> rectangles;
    
	findSquares(filtered, squares, rectangles, rectArray);
	//drawSquares(filtered, squares);
	//drawRectangles(filtered, rectangles);

	//cv::cvtColor(filtered, image, CV_GRAY2BGR565);

}

void RectangleFinder::findSquares(const cv::Mat& image,
								  std::vector<std::vector<cv::Point> >& squares,
								  std::vector<cv::Rect>& rectangles,
                                  int rectArray[]){
    squares.clear();
    rectangles.clear();

    cv::Mat pyr, timg, gray0(image.size(), CV_8U), gray;

    // down-scale and upscale the image to filter out the noise
    cv::pyrDown(image, pyr, cv::Size(image.cols/2, image.rows/2));
    cv::pyrUp(pyr, gray0, image.size());

    cv::dilate(gray0, gray0, diamond);
    //cv::erode(gray0, gray0, diamond);

    //cv::Canny(gray0, gray, 0, 50, 5);
    // dilate canny output to remove potential
    // holes between edge segments
    //cv::dilate(gray, image, diamond);
    //cv::erode(gray0, gray0, diamond);

    // find contours and store them all as a list
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(gray0, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    //filterRectangles(contours, squares);
    boundingBoxes(contours, rectangles);
    
    if(rectangles.size() > 0){
        cv::Rect r = rectangles.at(0);
        rectArray[0] = 1;
        rectArray[1] = r.x + r.width / 2;
        rectArray[2] = r.y + r.height / 2;
        rectArray[3] = 1;
        rectArray[4] = 1;
        
    }else {
        rectArray[0] = 0;
    }
}

void RectangleFinder::boundingBoxes(std::vector<std::vector<cv::Point> > contours,
									   std::vector<cv::Rect>& rectangles){

	for(int i = 0; i < contours.size(); i++){
		std::vector<cv::Point> contour = contours[i];
		float area = cv::contourArea(contour);
		//LOGD("area: %f", area);
		if(area > 5000){
			cv::Rect rect = cv::boundingRect(cv::Mat(contour));
			rectangles.push_back(rect);
            return;
		}
	}

}

void RectangleFinder::filterRectangles(std::vector<std::vector<cv::Point> > contours,
									   std::vector<std::vector<cv::Point> >& squares){

	std::vector<cv::Point> approx;

	// test each contour
	for( size_t i = 0; i < contours.size(); i++ ){
		// approximate contour with accuracy proportional
	    // to the contour perimeter
	    cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.02, true);

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
	        }
	    }
	}

}

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
double RectangleFinder::angle(cv::Point pt1, cv::Point pt2, cv::Point pt0){
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2) / std::sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

// the function draws all the squares in the image
void RectangleFinder::drawSquares( cv::Mat& image, const std::vector<std::vector<cv::Point> >& squares )
{
	LOGD("squares: %i", squares.size());
    for( size_t i = 0; i < squares.size(); i++ ){
        const cv::Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        cv::polylines(image, &p, &n, 1, true, cv::Scalar(255,255,255), 3, CV_AA);
    }

}

void RectangleFinder::drawRectangles(cv::Mat& image, const std::vector<cv::Rect>& rectangles){

	LOGD("rectangles: %i", rectangles.size());
	for(int i = 0; i < rectangles.size(); i++){
		cv::Rect rectangle = rectangles.at(i);
		cv::rectangle(image, rectangle, cv::Scalar(255,255,255), 2);
	}

}

void RectangleFinder::gendiamond(cv::Mat& dst, std::vector<int>& list){

	int size = list.size();
	int element;
	for(int i = 0; i < size; i++){
		element = (size - list[i]) / 2;
		for(int j = 0; j < element; j++){
			dst.at<uchar>(i, j) = 0;
			dst.at<uchar>(i, size - 1 - j) = 0;
		}
	}
}

/*
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
*/

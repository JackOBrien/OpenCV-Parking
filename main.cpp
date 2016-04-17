#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace cv;
using namespace std;

/// Global variables

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratios = 3;
int kernel_size = 3;

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
    /// Reduce noise with a kernel 3x3
    blur( src_gray, detected_edges, Size(3,3) );
    
    /// Canny detector
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold * ratios, kernel_size );
    
    /// Using Canny's output as a mask, we display our result
    dst = Scalar::all(0);
    
    src.copyTo( dst, detected_edges);
    imshow( "Edge Map", dst );
}


/** @function main */
int main( int argc, char** argv )
{
    
    VideoCapture cap("/Users/jacob/Desktop/DJI_0102.MOV"); // open the video file for reading
    
    if ( !cap.isOpened() )  // if not success, exit program
    {
        cout << "Cannot open the video file" << endl;
        return -1;
    }
    
    //cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms
    
    double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
    
    cout << "Frame per seconds : " << fps << endl;
    
    namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    
    namedWindow("grayScale",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

    
    /// Create a window
    namedWindow( "Edge Map", CV_WINDOW_AUTOSIZE );
    
    /// Create a Trackbar for user to enter threshold
    createTrackbar( "Min Threshold:", "Edge Map", &lowThreshold, max_lowThreshold, CannyThreshold );
    
    while(1)
    {
        Mat regionOfInterest;
        
        Rect interestRegion;
        interestRegion.width = 80;
        interestRegion.height = 165;
        interestRegion.x = 420;
        interestRegion.y = 380;
        
        bool bSuccess = cap.read(src); // read a new frame from video
        
        
        if (!bSuccess) //if not success, break loop
        {
            cout << "Cannot read the frame from video file" << endl;
            break;
            
        }
        
        
        resize(src, src, Size(1024, 768), 0, 0, INTER_CUBIC);
        regionOfInterest = src(interestRegion).clone();
        
        /// Create a matrix of the same type and size as src (for dst)
        dst.create( src.size(), src.type() );
        
        /// Convert the image to grayscale
        cvtColor( src, src_gray, CV_BGR2GRAY );
        
        /// Show the image
        CannyThreshold(0, 0);
        
        imshow("Ford Fusion", regionOfIntrest);
        
        imshow("MyVideo", src); //show the frame in "MyVideo" window
        
        if(waitKey(1) == 0) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }
    
    return 0;
    
}
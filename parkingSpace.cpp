/*----------------------------------------------------------------------*
 * parkingSpace Class			                       				    *
 *----------------------------------------------------------------------*
 *This class is used to model open and taken parking spaces with opencv.*
 *----------------------------------------------------------------------*
 * Project: Parking advisor	               		                        *
 * Author : McKim Jacob & Jack O'Brien                                  *
 * Date Of Creation: 4 - 12 - 2016                                      *
 * Revision : 1.00                                                      *
 *----------------------------------------------------------------------*/

// =================================================================== //
// A. Includes														   //
// =================================================================== //
#include "parkingSpace.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>



// =================================================================== //
// B. Class Definition												   //
// =================================================================== //

class parkingSpace {
    
    public:
    
    // == Public Vars ========================================= //
    
    /* (N/A)												 	*/
    
    // == Public Methods ====================================== //
    
    // -- Constructors -- //
    
    parkingSpace () {
        
    }
    
    parkingSpace (cv::String name, cv::Rect location) {
        setRegion(location.x, location.y, location.width, location.height);
    }
    
    // -- Core Methods -- //
    
    /* Method used to update the status of the parking space.   */
    void processFrame (cv::Mat frame);
    
    /* Returns whether or not the parking space is open.        */
    bool isSpaceAvailable ();
    
    
    // -- Accessors -- //
    
    /* Gets the extracted image of the parking space.           */
    cv::Mat getBaseFrame ();
    
    /* Grabs the image grayscaled frame.                        */
    cv::Mat getGrayscaleFrame ();
    
    /* Grabs the image that contains a list of all contours     */
    cv::Mat getContourMapFrame ();
    
    /* Gets the name of the spot.                               */
    cv::String getName ();
    
    /* Grabs the location data of the parkign space.            */
    cv::Rect getRegion ();
    
    // -- Mutators -- //
    
    /* Configures the region of where the space is.             */
    void setRegion (int x, int y, int width, int height);
    
    /* Configures the amount of time the program should wait.   */
    void setTime (int time);
    
    /* Sets the name of the spot to something.                  */
    void setName (cv::String name);
    
    private:
    
    // == Private Vars ======================================== //
    
    /* The name of the sopt.                                    */
    cv::String spot_name;
    
    /* The threshold number of contours the image can have.     */
    int contour_threshold_count = 4;
    
    /* Whether or not there is a car parked.                    */
    bool car_parked = false;
    
    /* Whether or not an object has been detected in the spot.  */
    bool object_detected = false;
    
    /* The amount of time in milliseconds the car has to park.  */
    int parked_car_time = 30000;
    
    /* The timer class used to monitor the parking time.        */
    //clock
    
    /* The rectangle area used to select space of intrest.      */
    cv::Rect interest_region;
    
    /* The striped image region we have cropped out.            */
    cv::Mat base_frame;

    /* The stripped image region with a grayscale filter.       */
    cv::Mat base_gs_frame;
    
    /* The stripped image region with a grayscale filter.       */
    cv::Mat edges_frame;
    
    /* The list of contours found in the image.                */
    std::vector<std::vector<cv::Point> > frame_contours;
    
    /* Information regarding each contour.                     */
     std::vector<cv::Vec4i> frame_hierarchy;
    
    // == Private Methods ===================================== //
    
    /* determines if the car has been parked long enough.       */
    bool checkTimer ();
    
    /* Resets the timer when a parking space becomes available. */
    void resetTimer ();
    
    /* Strips out the parking space from the base image.        */
    void stripImage (cv::Mat parking_frame);
    
};


// =================================================================== //
// C. Core Method Definitions									       //
// =================================================================== //

void parkingSpace::processFrame (cv::Mat frame) {
    
    // --- Local Variables ---------------------- //
    
    /* Used in the the computation of the threshold. (upper to lower)*/
    int ratio = 3;
    
    /* The threshold to uphold the edge detection to. */
    int lowThreshold = 88;
    
    /* The size of soble kernel used in canny algorithim. */
    int kernel_size = 3;
    
    // --- Core Routine ------------------------- //
    
    // 1. Strip the frame from the image.
    stripImage(frame);
    
    
    // 2. Filter and process the image into a form we can evaluate.
    cvtColor(base_frame, base_gs_frame, CV_BGR2GRAY);
    
    
    // 3. Preform edge detection for the space.

        // i.) pre-process the image by reducing the image noise.
    blur( base_gs_frame, edges_frame, cv::Size(3,3) );
    
        // ii.) Preform canny edge detection on the image.
    Canny(edges_frame, edges_frame, lowThreshold, lowThreshold * ratio, kernel_size);

        // iii.) Find the contours in the image and store them.
    findContours(edges_frame, frame_contours, frame_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
    

    // 4. Set the car parked variable to the state of the space.
    object_detected =frame_contours.size() >= contour_threshold_count ? true : false;
    
}


bool parkingSpace::isSpaceAvailable() {
    
    // todo
    return object_detected;
    
}


// =================================================================== //
// D. Accessor/Mutator Method Definitions							   //
// =================================================================== //

cv::String parkingSpace::getName () {
    return spot_name;
    
}

cv::Rect parkingSpace::getRegion() {
    return interest_region;
    
}


cv::Mat parkingSpace::getBaseFrame() {
    return base_frame;
    
}



cv::Mat parkingSpace::getGrayscaleFrame() {
    return base_gs_frame;
    
}

 
cv::Mat parkingSpace::getContourMapFrame() {
    
    // --- Local Variables ---------------------- //
    
    /* The matrix we will draw the contour data too.*/
    cv::Mat contour_drawing;
    
    /* This is used to generate the random colors. */
    cv::RNG rng(12345);
    
    /* The color to be used for the given contour. */
    cv::Scalar contour_color;
    
    // --- Core Routine ------------------------- //
    
    // 1. Format the matrix.
    contour_drawing = cv::Mat::zeros(edges_frame.size(), CV_8UC3);
    
    // 2. Loop through each contour and add it to the drawing.
    for( int i = 0; i < frame_contours.size(); i++ )
    {
        contour_color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
        drawContours( contour_drawing, frame_contours, i, contour_color, 2,
                                        8, frame_hierarchy, 0, cv::Point() );
    }
    
    // 3. Return the result image.
    return contour_drawing;

}



void parkingSpace::setRegion(int x, int y, int width, int height) {
    interest_region.x = x;
    interest_region.y = y;
    interest_region.width = width;
    interest_region.height = height;
    
}



void parkingSpace::setTime(int time) {
    if (time > 0)
        parked_car_time = time;
    
}

void parkingSpace::setName(cv::String name) {
    spot_name = name;
}


// =================================================================== //
// E. Helper Method Definitions                                        //
// =================================================================== //

bool parkingSpace::checkTimer() {
    return false;
    // todo
}



void parkingSpace::resetTimer() {
    // todo
}



void parkingSpace::stripImage(cv::Mat parking_frame) {
    base_frame = NULL;
    base_frame = parking_frame(interest_region).clone();
    
}

// =================================================================== //

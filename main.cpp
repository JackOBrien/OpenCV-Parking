/*----------------------------------------------------------------------*
 * Main.cpp                                                             *
 *----------------------------------------------------------------------*
 *This file is the core routine for monitoring avaliable parking spaces.*
 *----------------------------------------------------------------------*
 * Project: Parking advisor	               		                        *
 * Author : McKim Jacob & Jack O'Brien                                  *
 * Date Of Creation: 4 - 12 - 2016                                      *
 * Revision : 1.00                                                      *
 *----------------------------------------------------------------------*/


// =================================================================== //
// A. Includes														   //
// =================================================================== //

/* NOTE: Change this to path of video we want to process. */
#define VIDEO_PATH "/Users/jacob/Desktop/DroneVideo.mp4"


#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "parkingSpace.cpp"
using namespace cv;
using namespace std;

// =================================================================== //
// B. Helper Methods                                                   //
// =================================================================== //

Mat drawSpaceStatus (Mat imageFrame, parkingSpace space) {
    
    // --- Local Variables ------------------------------ //
    
    /* How big the circles radius should be in pixels.    */
    int circleRadius = 25;
    
    /* The color to use when drawing the spot data.       */
    Scalar status_color;
    
    /* The rectangle used to place circles center.        */
    Rect circle_config;
    
    /* The center of the circle.                          */
    Point circle_center;
    
     
    // --- Core Routine --------------------------------- //
    
    
    // 1. Determine whether or not the space is open.
    if (space.isSpaceAvailable())
        status_color = Scalar(0,255,0);
    else
        status_color = Scalar(0,0,255);
        
    // 2. find circle center.
    circle_config = space.getRegion();
    circle_center.x = circle_config.x + (circle_config.width / 2);
    circle_center.y = circle_config.y + (circle_config.height / 2);
    
    // 2. Draw the spot information to the screen.
    rectangle(imageFrame, circle_config, status_color);
    circle(imageFrame, circle_center, circleRadius, status_color);
    circle_center.y += circleRadius + 15;
    circle_center.x -= 30;
    putText(imageFrame, space.getName(), circle_center, FONT_HERSHEY_SIMPLEX, .5, status_color);
    
    
    return imageFrame;
    
}


// =================================================================== //
// C. Core Routine                                                     //
// =================================================================== //


/** @function main */
int main( int argc, char** argv )
{
    // --- Local Variables ------------------------------ //
    
    /* Variable used to draw the contours of the whole image. */
    parkingSpace contour_map = *new parkingSpace();
    
    /* The video stream that is going to be monitored.    */
    VideoCapture cctv_stream(VIDEO_PATH);
    
    /* The frame pulled from the video stream to process. */
    Mat video_frame;
    
    /* The frame used to draw the UI elements on.         */
    Mat ui_frame;
    
    /* The array of spaces that are monitored.            */
    parkingSpace spaces[24];
    
    /* The total number of spaces open.                   */
    int space_tally = 0;
    
    // --- Core Routine --------------------------------- //
    
    // 1. Open stream for review.
    if (!cctv_stream.isOpened())
    {
        cout << "I/O ERROR: Cannot open the video file." << endl;
        return -1;
    }
    
    // 2. Define parking region objects.
    
    // Row 1
    spaces[0].setRegion(82,200,70,140);
    spaces[0].setName("Spot 1");
    spaces[1].setRegion(170,200,70,140);
    spaces[1].setName("Spot 2");
    spaces[2].setRegion(260,200,70,140);
    spaces[2].setName("Spot 3");
    spaces[3].setRegion(370,200,70,140);
    spaces[3].setName("Spot 4");
    spaces[4].setRegion(470,200,70,140);
    spaces[4].setName("Spot 5");
    spaces[5].setRegion(560,200,70,140);
    spaces[5].setName("Spot 6");
    spaces[6].setRegion(660,200,70,140);
    spaces[6].setName("Spot 7");
    spaces[7].setRegion(760,200,70,140);
    spaces[7].setName("Spot 8");
    spaces[8].setRegion(860,200,70,140);
    spaces[8].setName("Spot 9");
    spaces[9].setRegion(960,200,70,140);
    spaces[9].setName("Spot 10");
    spaces[10].setRegion(1060,200,70,140);
    spaces[10].setName("Spot 11");
    spaces[11].setRegion(1150,200,70,140);
    spaces[11].setName("Spot 12");
    
    // Row 2
    spaces[12].setRegion(50,400,70,140);
    spaces[12].setName("Spot 13");
    spaces[13].setRegion(150,400,70,140);
    spaces[13].setName("Spot 14");
    spaces[14].setRegion(250,400,70,140);
    spaces[14].setName("Spot 15");
    spaces[15].setRegion(350,400,70,140);
    spaces[15].setName("Spot 16");
    spaces[16].setRegion(470,400,70,140);
    spaces[16].setName("Spot 17");
    spaces[17].setRegion(570,400,70,140);
    spaces[17].setName("Spot 18");
    spaces[18].setRegion(670,400,80,140);
    spaces[18].setName("Spot 19");
    spaces[19].setRegion(775,400,70,140);
    spaces[19].setName("Spot 20");
    spaces[20].setRegion(875,400,70,140);
    spaces[20].setName("Spot 21");
    spaces[21].setRegion(975,400,70,140);
    spaces[21].setName("Spot 22");
    spaces[22].setRegion(1075,400,70,140);
    spaces[22].setName("Spot 23");
    spaces[23].setRegion(1175,400,70,140);
    spaces[23].setName("Spot 24");

    
    // 3. Create UI components for display.
    namedWindow("Raw Feed",CV_WINDOW_AUTOSIZE);
    namedWindow("Parking Feed Monitor",CV_WINDOW_AUTOSIZE);
    namedWindow("region",CV_WINDOW_AUTOSIZE);
    
    // 4. Loop each frame of the video.
    while (true) {
        
        // I. Read in the next frame of the stream.
        if (!cctv_stream.read(video_frame)) {
            cout << "STREAM ERROR: Unable to read video frame. Program exiting." << endl;
            break;
        }
        resize(video_frame, video_frame, Size(1280, 720), 0, 0, INTER_CUBIC);
        imshow("Raw Feed", video_frame);
        ui_frame = video_frame;
        
        // Demo Purposes.
        contour_map.setRegion(0,0,1280,720);
        contour_map.setName("Contour Map");
        contour_map.processFrame(video_frame);
        
        
        // II. Call each parking space object to check for cars & Update screen draw data. (note takes into account contour map)
        for (int i = 0; i < sizeof(spaces)/sizeof(*spaces); i++) {
            spaces[i].processFrame(video_frame);
            ui_frame = drawSpaceStatus(ui_frame, spaces[i]);
            if (spaces[i].isSpaceAvailable()) {
                space_tally++;
            }
        }
        
        
        // III. Draw the UI.
        putText(ui_frame, to_string(space_tally) + " Spaces Available out of 24.",  Point(20,20),
                FONT_HERSHEY_SIMPLEX, .6, Scalar(0,255,0));
        imshow("Contour Map", contour_map.getContourMapFrame());
        imshow("Parking Feed Montior", ui_frame);

        
        // IV. offer a means of executing the loop.
        if(waitKey(2) == 27) {
            cout << "Program Prompted to exit." << endl;
            break;
        }
        
        
        // V. Reset the tally.
        space_tally = 0;
        
    }
    
    return 0;
    
}

// =================================================================== //

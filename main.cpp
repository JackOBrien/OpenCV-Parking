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
    
    //if (&space != nullptr) {
    
        // 1. Determine whether or not the space is open.
        if (space.isSpaceAvailable())
            status_color = Scalar(0,255,0);
        else
            status_color = Scalar(0,0,255);
        
        // 2. find circle center.
        circle_config = space.getRegion();
        circle_center.x = circle_config.x + (circle_config.width / 2);
    cout <<circle_center.x << endl;
        circle_center.y = circle_config.y + (circle_config.height / 2);
    
        // 2. Draw the spot information to the screen.
        rectangle(imageFrame, circle_config, status_color);
        circle(imageFrame, circle_center, circleRadius, status_color);
        circle_center.y += circleRadius + 15;
        circle_center.x -= 20;
        putText(imageFrame, space.getName(), circle_center, FONT_HERSHEY_SIMPLEX, .5, status_color);
    //}
    
    return imageFrame;
    
}


// =================================================================== //
// C. Core Routine                                                     //
// =================================================================== //


/** @function main */
int main( int argc, char** argv )
{
    // --- Local Variables ------------------------------ //
    
    /* The video stream that is going to be monitored.    */
    VideoCapture cctv_stream("/Users/jacob/Desktop/d.mp4");
    
    /* The frame pulled from the video stream to process. */
    Mat video_frame;
    
    /* The frame used to draw the UI elements on.         */
    Mat ui_frame;
    
    /* The array of spaces that are monitored.            */
    parkingSpace spaces[16];
    
    // --- Core Routine --------------------------------- //
    
    // 1. Open stream for review.
    if (!cctv_stream.isOpened())
    {
        cout << "I/O ERROR: Cannot open the video file." << endl;
        return -1;
    }
    
    // 2. Define parking region objects.
    spaces[0].setRegion(82,200,70,140);
    spaces[0].setName("Spot 1");
    spaces[1].setRegion(170,200,70,140);
    spaces[1].setName("Spot 2");
    spaces[2].setRegion(390,240,50,100);
    spaces[2].setName("Spot 3");
    spaces[3].setRegion(470,240,50,100);
    spaces[3].setName("Spot 4");
    spaces[4].setRegion(550,240,50,100);
    spaces[4].setName("Spot 5");
    spaces[5].setRegion(620,240,50,100);
    spaces[5].setName("Spot 6");
    spaces[6].setRegion(690,240,50,100);
    spaces[6].setName("Spot 7");
    spaces[7].setRegion(760,240,50,100);
    spaces[7].setName("Spot 8");
    spaces[8].setRegion(830,240,50,100);
    spaces[8].setName("Spot 9");
    spaces[9].setRegion(900,240,50,100);
    spaces[9].setName("Spot 10");
    spaces[10].setRegion(970,240,50,100);
    spaces[10].setName("Spot 11");
    spaces[11].setRegion(1050,240,50,100);
    spaces[11].setName("Spot 12");
    spaces[12].setRegion(1120,240,50,100);
    spaces[12].setName("Spot 13");
    spaces[13].setRegion(1200,240,50,100);
    spaces[13].setName("Spot 14");
    spaces[14].setRegion(230,390,50,100);
    spaces[14].setName("Spot 15");
    spaces[15].setRegion(300,390,50,100);
    spaces[15].setName("Spot 16");
    //spaces[14].setRegion(1270,240,50,100);
    //spaces[14].setName("Spot 15");
    
    // 3. Create UI components for display.
    namedWindow("Raw Feed",CV_WINDOW_AUTOSIZE);
    namedWindow("Parking Feed Monitor",CV_WINDOW_AUTOSIZE);
    namedWindow("region",CV_WINDOW_AUTOSIZE);
    // todo
    
    // 4. Loop each frame of the video.
    while (true) {
        
        // I. Read in the next frame of the stream.
        if (!cctv_stream.read(video_frame)) {
            cout << "STREAM ERROR: Unable to read video frame. Program exiting." << endl;
            break;
        }
        resize(video_frame, video_frame, Size(1280, 720), 0, 0, INTER_CUBIC);
        ui_frame = video_frame;
        
        // II. Call each parking space object to check for cars & Update screen draw data.
        for (int i = 0; i < sizeof(spaces)/sizeof(*spaces); i++) {
            spaces[i].processFrame(video_frame);
            ui_frame = drawSpaceStatus(ui_frame, spaces[i]);
        }
        
        // III. Draw the UI.
        imshow("region", spaces[1].getContourMapFrame());
        imshow("Raw Feed", video_frame);
        imshow("Parking Feed Montior", ui_frame);
        
        
        // IV. offer a means of executing the loop.
        if(waitKey(2) == 27) {
            cout << "Program Prompted to exit." << endl;
            break;
        }
        
    }
    
    return 0;
    
}

// =================================================================== //

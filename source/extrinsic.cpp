#include <opencv2/opencv.hpp>

#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>
using namespace std;
using namespace cv;
/*
Mat CreateTestImage() {
    ret = Mat3f(500,500,0.0f)
    circle(ret,Point(250,250),150)
}
*/

int main()
{
    char str[50];

    Mat img, draw;

    // -- USE CAMERA --

    // initlaize video capture 
    VideoCapture cap;
    // open the default camera using default API
    //cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;             // 0 = open default camera
    // read up on cv::MORPH_ kernels for eroding and dilating

    // -- END USE CAMERA --

    Mat kern = getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3), cv::Point(-1, -1));

    while (1) {
        // -- USE CAMERA --

        // wait for a new frame from camera and store it into 'frame'
        //cap.read(img);

        // -- END USE CAMERA --


        // -- USE SAVED IMAGE --

        //img = imread("./images/1.jpg", 1);
        img = imread("./images/2.jpg", 1);
        //img = imread("./images/3.jpg", 1);

        // -- END USE SAVED IMAGE --
        // safety check to see if img has stuff
        if (img.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        draw = img;

        // Convert to gray scale, but for tennis balls, you probably dont want to do this
        //cvtColor(img, img, COLOR_BGR2GRAY);

        // threshold pixels with a value of 250 or greater - these values will need to be tuned. Start broad and close it down
        //inRange(img, 150, 255, img);
        // example of thresholdign in color: BGR
        inRange(img, Scalar(0, 100, 50), Scalar(200, 255, 255), img);
        imshow("Threshold", img);

        // erode and dilate once
        erode(img, img, kern, Point(-1,-1), 1);
        dilate(img, img, kern, Point(-1,-1), 1);

        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours(img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

        vector<Vec3f> circles;
        cout << circles.size() << " Tennis Balls were found in the image.\n";

        int largestApparentRadius = 0;
        Point closest;

        for( size_t i = 0; i < circles.size(); i++ )
        {
            Vec3i c = circles[i];
            Point center = Point(c[0], c[1]);
            // circle center
            circle( draw, center, 1, Scalar(0,100,100), 3, LINE_AA);
            // circle outline
            int radius = c[2];
            if (radius>largestApparentRadius){
                largestApparentRadius = radius;
                closest = center;
            }

            circle( draw, center, radius, Scalar(255,0,255), 3, LINE_AA);
        }

        HoughCircles(img, circles, HOUGH_GRADIENT, 1,
                     img.rows/16,  // change this value to detect circles with different distances to each other
                     100, 30, 1, img.rows // change the last two parameters
                // (min_radius & max_radius) to detect larger circles
        );

        for (size_t i = 0; i < contours.size(); i++) {


            //drawContours(draw, contours, (int)i, Scalar(0, 255, 0), 2, LINE_8, hierarchy, 0);

        }

        // Display image
        imshow("Final", draw);
        
        // wait for key stroke
        char c = waitKey(10);

        if (c == 27) {
            break;
        }
    }
}

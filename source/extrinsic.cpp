#include <opencv2/opencv.hpp>

#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>
using namespace std;
using namespace cv;

const float PI = 3.14159;
/* COLORS DO NOT GET PROPERLY SET!
Mat CreateTestImage() {
    Mat ret = Mat3f(500,500,0.0f);
    circle(ret,Point(250,250),150,Scalar(50,200,150),-1,LINE_AA);//-1 means fill
    return ret;
}
*/

int numCircles;
float radius,maxApparentRadius;
Point2f center,closeCenter;

int main()
{
    char str[50];

    Mat img, imgHSV,draw;

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

        //img=CreateTestImage();
        //img = imread("./images/1.jpg", 1);
        //img = imread("./images/2.jpg", 1);
        img = imread("./images/3.jpg", 1);

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
        //inRange(img, Scalar(0, 100, 50), Scalar(200, 255, 255), img);

        //HSV Thresholding
        cvtColor(img, imgHSV, COLOR_BGR2HSV);

        imshow("HSV as RGB", imgHSV);

        inRange(imgHSV, Scalar(14,70,88), Scalar(50,254,255), img);

        imshow("Threshold", img);

        // erode and dilate twice
        erode(img, img, kern, Point(-1,-1), 2);
        dilate(img, img, kern, Point(-1,-1), 2);

        //dilate and erode twice
        dilate(img, img, kern, Point(-1,-1), 2);
        erode(img, img, kern, Point(-1,-1), 2);

        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours(img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));


        maxApparentRadius = 0;
        numCircles = 0;

        for (size_t i = 0; i < contours.size(); i++) {
            drawContours(draw, contours, (int)i, Scalar(0, 255, 0), 2, LINE_8, hierarchy, 0);

            minEnclosingCircle(contours[i], center,radius);

            float circleArea = PI * radius * radius;
            float actualArea = contourArea(contours[i]); //contour area

            double circleness = actualArea/circleArea; //aproaches 1 as shape becomes more circular
            if (circleness < 0.9){
                continue;
            }

            numCircles++;
            if (radius>maxApparentRadius){
                closeCenter=center;
                maxApparentRadius = radius;
            }
        }

        // Display image
        imshow("Final", draw);

        // wait for key stroke
        char c = waitKey(10);

        if (c == 27) {
            break;
        }
    }

    cout << numCircles << " " << "Tennis balls detected." << " ";
    if(numCircles>0){
        cout << "The closest tennis ball was at " << closeCenter << ".\n";
    }
}

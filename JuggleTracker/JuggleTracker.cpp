#include<opencv2/opencv.hpp>
#include<windows.h>
#include<iostream>

#include "cropWindow.h"

using namespace std;
using namespace cv;
using namespace cropwin;

const int max_value_H = 360 / 2;
const int max_value = 255;

const bool BLUR = false;


int main()
{
	
	const string winTresh = "Treshold", winColor = "Color", winControls = "Settings";

	//Set SimpleBlobDetector parameters
	SimpleBlobDetector::Params params;
	params.filterByArea = true;
	params.filterByCircularity = false;
	// params.minCircularity = .7f;
	// params.maxCircularity = 1;
	params.filterByConvexity = false;
	params.filterByInertia = false;
	params.filterByColor = true;
	params.blobColor = 255;
	params.minArea = 200;
	params.maxArea = 1000000;
	
	params.minThreshold = 200;
	params.maxThreshold = 255;
	params.thresholdStep = 20;

	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);


	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cout << "Failed to open webcam..." << endl;
		cin.get();
		return -1;
	}

	CropWindow cropWindow(winColor, WINDOW_AUTOSIZE);
	namedWindow(winTresh, WINDOW_AUTOSIZE);
	namedWindow(winControls, WINDOW_NORMAL);

	// int treshold = 70;
	int blurSize = 5;
	int low_H = 30, low_S = 50, low_V = 30;
	int high_H = 110, high_S = max_value, high_V = max_value;

	// createTrackbar("treshold", winControls, &treshold, 255);

	if (BLUR)
	{
		createTrackbar("blur size", winControls, &blurSize, 30);
	}


	createTrackbar("Low H", winControls, &low_H, max_value_H);
	createTrackbar("High H", winControls, &high_H, max_value_H);

	createTrackbar("low S", winControls, &low_S, max_value);
	createTrackbar("high S", winControls, &high_S, max_value);
	createTrackbar("low V", winControls, &low_V, max_value);
	createTrackbar("high V", winControls, &high_V, max_value);

	while (true)
	{
		Mat frame, cropped, hsv, gray, bgr[3];
		cap >> frame; // get a new frame from camera
		
		
		cropWindow.setImage(frame);

		// grey = cropWindow.getCropped();
		hsv = cropWindow.getCropped();

		if (BLUR)
		{
			int bSize = 2 * blurSize + 1;
			GaussianBlur(hsv, hsv, Size(bSize, bSize), 1.5, 1.5);
		}
		

		cvtColor(hsv, hsv, COLOR_BGR2HSV);
		
		inRange(hsv, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), gray);

		// split(cropWindow.getCropped(), bgr);
		// gray = bgr[1];




		
		// threshold(grey, grey, treshold, 255, THRESH_BINARY_INV);

		

		vector<cv::KeyPoint> keypoints;
		detector->detect(gray, keypoints);

		Mat im_with_keypoints;
		drawKeypoints(cropWindow.getCropped(), keypoints, im_with_keypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

		// Show blobs
		imshow("keypoints", im_with_keypoints);
		imshow(winTresh, gray);
		cropWindow.show();

		if (waitKey(10) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}
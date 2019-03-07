#include<opencv2/opencv.hpp>
#include<iostream>

#include "cropWindow.h"

using namespace std;
using namespace cv;
using namespace cropwin;



int main()
{


	const string winTresh = "Treshold", winColor = "Color", winTrack = "Settings";

	//Set SimpleBlobDetector parameters
	SimpleBlobDetector::Params params;
	params.filterByArea = true;
	params.filterByCircularity = false;
	params.filterByConvexity = false;
	params.filterByInertia = false;
	params.filterByColor = true;
	params.blobColor = 255;
	params.minArea = 100;

	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

	VideoCapture cap(1);
	if (!cap.isOpened())
	{
		cout << "Failed to open webcam..." << endl;
		cin.get();
		return -1;
	}

	CropWindow cropWindow(winColor, WINDOW_AUTOSIZE);
	namedWindow(winTresh, WINDOW_AUTOSIZE);
	namedWindow(winTrack, WINDOW_AUTOSIZE);

	int treshold = 70;
	int blurSize = 5;
	createTrackbar("treshold", winTrack, &treshold, 255);
	createTrackbar("blur size", winTrack, &blurSize, 30);
	

	while (true)
	{
		Mat frame, gray, bgr[3];
		cap >> frame; // get a new frame from camera
		
		
		cropWindow.setImage(frame);

		// grey = cropWindow.getCropped();

		cvtColor(cropWindow.getCropped(), gray, COLOR_BGR2GRAY);
		gray = bgr[1];

		// std::vector<KeyPoint> keypoints;
		// detector.detect(frame, keypoints);

		int bSize = 2*blurSize + 1;
		// GaussianBlur(green, green, Size(bSize, bSize), 1.5, 1.5);
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
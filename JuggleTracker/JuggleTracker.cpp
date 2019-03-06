#include<opencv2/opencv.hpp>
#include<iostream>

#include "cropWindow.h"

using namespace std;
using namespace cv;
using namespace cropwin;

int main()
{
	const string winTresh = "Treshold", winColor = "Color", winTrack = "Settings";

	VideoCapture cap(0);
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
		Mat frame, grey;
		cap >> frame; // get a new frame from camera
		
		
		cropWindow.setImage(frame);

		// grey = cropWindow.getCropped();
		cvtColor(cropWindow.getCropped(), grey, COLOR_BGR2GRAY);

		// std::vector<KeyPoint> keypoints;
		// detector.detect(frame, keypoints);

		int bSize = 2*blurSize + 1;
		GaussianBlur(grey, grey, Size(bSize, bSize), 1.5, 1.5);
		threshold(grey, grey, treshold, 255, THRESH_BINARY_INV);

		imshow(winTresh, grey);
		if (waitKey(10) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}
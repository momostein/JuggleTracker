// Uncomment to add gaussian blur (warning takes a lot of performance)
// #define BLUR

// Uncomment to enable cropping
// #define CROPPING

#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>

#include "tracker.h"
#include "graphics.h"


#ifdef CROPPING
#include "cropWindow.h"
#endif // CROPPING


using namespace std;
using namespace cv;
using namespace tracker;
using namespace graphics;

#ifdef CROPPING
using namespace cropwin;
#endif // CROPPING

const int max_value_H = 360 / 2;
const int max_value = 255;

sf::Mutex myMutex;
ThingManager things(myMutex);

void renderingThread(sf::RenderWindow* window);

int main()
{
	// ------------------------------
	//		OPENCV
	// ------------------------------

	// Window names
	const string winTresh = "Treshold", winColor = "Color", winControls = "Settings", winKeyPoints = "Objects";

	// Tracker parameters
	const int maxLostFrames = 20;	// Maximum frames a missing object will persist
	const int maxDist = 700;		// Maximum distance between keypoints to be considered the same

	// Windows
	namedWindow(winTresh, WINDOW_AUTOSIZE);				// Window for checking the thresholded image
	namedWindow(winControls, WINDOW_NORMAL);			// Window with all the controls
	namedWindow(winKeyPoints, WINDOW_AUTOSIZE);			// Window with all the objects

#ifdef CROPPING
	CropWindow cropWindow(winColor, WINDOW_AUTOSIZE);	// For cropping the input
#endif // CROPPING


	// Set SimpleBlobDetector parameters
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
	params.maxArea = 100000000;

	params.minThreshold = 200;
	params.maxThreshold = 255;
	params.thresholdStep = 20;

	// Create blobdetector
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

	// Create tracker
	KeypointTracker tracker(&things, myMutex, maxDist, maxLostFrames);

	// Open camera
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cout << "Failed to open webcam..." << endl;
		cin.get();
		return -1;
	}

	// Sliders
	// int treshold = 70;
	int blurSize = 5;
	int low_H = 30, low_S = 60, low_V = 40;
	int high_H = 75, high_S = max_value, high_V = max_value;

	// createTrackbar("treshold", winControls, &treshold, 255);

#ifdef BLUR
	createTrackbar("blur size", winControls, &blurSize, 30);
#endif // BLUR


	createTrackbar("Low H", winControls, &low_H, max_value_H);
	createTrackbar("High H", winControls, &high_H, max_value_H);

	createTrackbar("low S", winControls, &low_S, max_value);
	createTrackbar("high S", winControls, &high_S, max_value);
	createTrackbar("low V", winControls, &low_V, max_value);
	createTrackbar("high V", winControls, &high_V, max_value);

	// ------------------------------
	//		SFML
	// ------------------------------

	// create the window (remember: it's safer to create it in the main thread due to OS limitations)
	sf::RenderWindow window(sf::VideoMode(800, 600), "TEST");

	// deactivate its OpenGL context
	window.setActive(false);

	// launch the rendering thread
	sf::Thread thread(&renderingThread, &window);
	thread.launch();

	while (window.isOpen())
	{
		Mat frame, hsv, thresh, bgr[3];
		cap >> frame; // get a new frame from camera

#ifdef CROPPING
		cropWindow.setImage(frame);
		cropWindow.getCropped().copyTo(frame);
		cropWindow.show();
#endif // CROPPING


#ifdef BLUR
		int bSize = 2 * blurSize + 1;
		GaussianBlur(hsv, hsv, Size(bSize, bSize), 1.5, 1.5);
#endif // BLUR


		cvtColor(frame, hsv, COLOR_BGR2HSV);

		inRange(hsv, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), thresh);

		vector<cv::KeyPoint> keypoints;
		detector->detect(thresh, keypoints);

		tracker.update(keypoints, frame.size());

		Mat im_with_keypoints;
		tracker.draw(frame, im_with_keypoints, Scalar(0, 0, 255));

		// Show blobs
		imshow(winKeyPoints, im_with_keypoints);
		imshow(winTresh, thresh);

		// OpenCV events
		if (waitKey(1) >= 0)
		{
			window.close();
		}
		// SFML events
		sf::Event event;

		// while there are pending events...
		while (window.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
			case sf::Event::Closed:
				window.close();
				break;

				// key pressed
			case sf::Event::KeyPressed:
				break;

				// we don't process other types of events
			default:
				break;
			}
		}
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	thread.wait();

	return 0;
}

void renderingThread(sf::RenderWindow* window)
{
	// activate the window's context
	window->setFramerateLimit(60);
	window->setActive(true);

	// the rendering loop
	while (window->isOpen())
	{
		window->clear();

		things.update();
		window->draw(things);


		// end the current frame
		window->display();
	}
}
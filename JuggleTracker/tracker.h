#pragma once
#include <opencv2/opencv.hpp>
#include "graphics.h"

namespace tracker {
	using namespace std;
	using namespace cv;
	using namespace graphics;

	class Object
	{
	private:
		unsigned int ID;
		ThingManager* graphMgr;
		JuggleThing* jug;

		sf::Mutex* myMutex;
		Point2f prevPos;

	public:
		KeyPoint pos;
		KeyPoint nextPos();

		int missing;
		void update(KeyPoint pt);
		void updateThing(const Size &matSize);

		void draw(InputOutputArray outImage, const Scalar & color);
		

		Object(
			ThingManager* graphMgr, sf::Mutex *myMutex,
			KeyPoint pos, unsigned int ID);

		~Object();
	};



	class KeypointTracker
	{
	private:
		list<Object*> objects_;
		ThingManager* graphMgr;
		sf::Mutex* myMutex;

		double maxDist;
		int maxMissing;
		unsigned int nextID;

	public:
		void update(vector<KeyPoint> keypoints, const Size &matSize);
		void draw(
			InputArray image, InputOutputArray outImage,
			const Scalar & color, DrawMatchesFlags flags = DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

		KeypointTracker(
			ThingManager* graphMgr, sf::Mutex &myMutex,
			double maxDist = 400, int maxMissing = 5) :
			graphMgr(graphMgr), myMutex(&myMutex),
			maxDist(maxDist), maxMissing(maxMissing) {};
	};


	// Calculate euclid distance
	double euclidDistance(const KeyPoint& pt1, const KeyPoint& pt2);

	// Find the index of nearest neighbor point from keypoints.
	int nearestNeighbor(const KeyPoint& pt1, const vector<KeyPoint>& keypoints, double treshold = 400);
}
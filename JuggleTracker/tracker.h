#pragma once
#include <opencv2/opencv.hpp>

namespace tracker {
	using namespace std;
	using namespace cv;

	class Object
	{
	private:
		unsigned int ID;

	public:
		vector<float> vel;
		KeyPoint pos;

		int missing;
		void update(KeyPoint pt);
		
		void draw(InputOutputArray outImage, const Scalar & color);

		Object(KeyPoint pos, unsigned int ID) : pos(pos), ID(ID) {};
	};



	class KeypointTracker
	{
	private:
		list<Object> objects_;

		double maxDist;
		int maxMissing;
		unsigned int nextID;

	public:
		void update(vector<KeyPoint> keypoints);
		const list<Object>& objects() const { return objects_; }

		void draw(InputArray image, InputOutputArray outImage, const Scalar & color, DrawMatchesFlags flags = DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

		KeypointTracker(double maxDist = 400, int maxMissing = 5) : maxDist(maxDist), maxMissing(maxMissing) {};
	};

	
	// Calculate euclid distance
	double euclidDistance(const KeyPoint& pt1, const KeyPoint& pt2);

	// Find the index of nearest neighbor point from keypoints.
	int nearestNeighbor(const KeyPoint& pt1, const vector<KeyPoint>& keypoints, double treshold = 400);
}
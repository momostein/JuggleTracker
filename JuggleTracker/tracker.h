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
		
		void draw(InputArray image, InputOutputArray outImage, const Scalar & color);

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


	/**
	 * Find pairs of points with the smallest distace between them
	 */
	void findPairs(
		const vector<KeyPoint>& keypoints1,
		const vector<KeyPoint>& keypoints2,
		vector<Point2f>& srcPoints,
		vector<Point2f>& dstPoints)
	{
		for (int i = 0; i < keypoints1.size(); i++) {
			KeyPoint pt1 = keypoints1[i];

			int nn = nearestNeighbor(pt1, keypoints2);
			if (nn >= 0) {
				KeyPoint pt2 = keypoints2[nn];
				srcPoints.push_back(pt1.pt);
				dstPoints.push_back(pt2.pt);
			}
		}
	}
}
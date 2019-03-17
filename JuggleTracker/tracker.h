#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

class KeypointTracker
{
private:
	list<Object> objectList;

public:
	void update(const vector<KeyPoint>& keypoints);

	const list<Object>& objects() const { return objectList; }

};

class Object
{
private:

public:

};

const double THRESHOLD = 400;

/**
 * Calculate euclid distance
 */
double euclidDistance(const KeyPoint& pt1, const KeyPoint& pt2) {
	double sum = 0.0;
	sum = pow(pt1.pt.x - pt2.pt.x, 2) + pow(pt1.pt.y - pt2.pt.y, 2);

	return sqrt(sum);
}

/**
 * Find the index of nearest neighbor point from keypoints.
 */
int nearestNeighbor(const KeyPoint& pt1, const vector<KeyPoint>& keypoints) {
	int neighbor = -1;
	double minDist = 1e6;

	for (int i = 0; i < keypoints.size(); i++) {
		KeyPoint pt2 = keypoints[i];
		double d = euclidDistance(pt1, pt2);
		//printf("%d %f\n", v.cols, d);

		if (d < minDist) {
			minDist = d;
			neighbor = i;
		}
	}

	if (minDist < THRESHOLD) {
		return neighbor;
	}

	return -1;
}

/**
 * Find pairs of points with the smallest distace between them
 */
void findPairs(const vector<KeyPoint>& keypoints1,
			   const vector<KeyPoint>& keypoints2,
			   vector<Point2f>& srcPoints, 
			   vector<Point2f>& dstPoints) {
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

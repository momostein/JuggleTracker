#include "tracker.h"


namespace tracker
{
	void KeypointTracker::update(vector<KeyPoint> keypoints)
	{
		for (auto itr = objects_.begin(); itr != objects_.end(); itr++)
		{
			int nn = nearestNeighbor(itr->pos, keypoints);
			if (nn >= 0)
			{
				// Found a corresponding keypoint
				itr->update(keypoints[nn]);
				keypoints.erase(keypoints.begin() + nn);
			}
			else
			{
				itr->missing++;
				if (itr->missing > maxMissing)
				{
					// Remove object if it has been missing for too long
					itr = objects_.erase(itr);
					
					// erase returns the next iterator so we go back a step
					itr--; 
				}
			}
		}
	}

	
	void Object::update(KeyPoint pt)
	{
		pos = pt;
		missing = 0;
	}

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
	int nearestNeighbor(const KeyPoint& pt1, const vector<KeyPoint>& keypoints, double treshold) {
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

		if (minDist < treshold) {
			return neighbor;
		}

		return -1;
	}
}


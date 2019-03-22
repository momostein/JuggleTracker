#include <string>
#include "tracker.h"


namespace tracker
{
	void KeypointTracker::update(const vector<KeyPoint>& keypoints, const Size &matSize)
	{
		for (auto const& kp : keypoints)
		{
			double minDist = 1e6;
			Object *selected = NULL;
			for (Object *i : objects_)
			{
				if (i->found)
					continue;

				double dist = euclidDistance(kp, i->pos);
				if (minDist > dist)
				{
					minDist = dist;
					selected = i;
				}
			}

			if ((minDist <= maxDist) && (selected != NULL))
			{
				selected->update(kp);
			}
			else 
			{
				sf::Lock lock(*myMutex);
				objects_.push_back(new Object(graphMgr, myMutex, kp, nextID++));
			}
		}

		auto itr = objects_.begin();
		while (itr != objects_.end())
		{
			sf::Lock lock(*myMutex);
			if (!(*itr)->found)
			{
				(*itr)->missing++;
				if ((*itr)->missing >= maxMissing)
				{
					delete *itr;
					itr = objects_.erase(itr);
					continue;
				}
			}

			(*itr)->updateThing(matSize);
			
			(*itr)->found = false;
			itr++;
		}
	}

	void KeypointTracker::draw(InputArray image, InputOutputArray outImage, const Scalar & color, DrawMatchesFlags flags)
	{
		// image.copyTo(outImage);

		vector<KeyPoint> keypoints(objects_.size());
		auto itr = objects_.begin();

		for (int i = 0; itr != objects_.end(); itr++, i++)
		{
			// it->draw(outImage, color);
			keypoints[i] = (*itr)->pos;
		}

		drawKeypoints(image, keypoints, outImage, color, flags);

		for (auto itr = objects_.begin(); itr != objects_.end(); itr++)
		{
			(*itr)->draw(outImage, color);
		}
	}


	void Object::update(KeyPoint pt)
	{
		pos = pt;
		missing = 0;
		found = true;
	}

	void Object::updateThing(const Size & matSize)
	{
		sf::Lock lock(*myMutex);
		jug->moveTo(pos.pt.x / matSize.width - 0.5f, pos.pt.y / matSize.height - 0.5f);
	}

	void Object::draw(InputOutputArray outImage, const Scalar & color)
	{
		circle(outImage, pos.pt, 2, color, -1);
		
		string text = " " + to_string(ID);
		int fontFace = FONT_HERSHEY_PLAIN;
		double fontScale = 1;
		int thickness = 2;

		// then put the text itself
		putText(outImage, text, pos.pt, fontFace, fontScale,
			color, thickness, 8);
	}

	inline Object::Object(ThingManager * graphMgr, sf::Mutex * myMutex, KeyPoint pos, unsigned int ID) : myMutex(myMutex), pos(pos), ID(ID)
	{
		jug = new JuggleThing();
		graphMgr->add(jug);
		found = true;
	}

	Object::~Object()
	{
		sf::Lock lock(*myMutex);
		jug->kill();
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


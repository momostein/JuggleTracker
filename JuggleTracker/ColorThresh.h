#pragma once
#include <opencv2/opencv.hpp>

namespace cFilter
{
	using namespace cv;

	void colorDist(InputArray _src, OutputArray _dst, const Vec3d & color, double range);
	
}
#include "ColorThresh.h"

namespace cFilter
{
	void colorDist(InputArray _src, OutputArray _dst, const Vec3d & color, double range)
	{
	
		if (_src.empty())
		{
			_src.copyTo(_dst);
			return;
		}

		Mat src = _src.getMat();
		_dst.create(src.size(), src.type());
		Mat dst = _dst.getMat();

		
		for (int row = 0; row < src.rows; row++)
		{
			for (int col = 0; col < src.cols; col++)
			{
				Vec3d pixel = src.at<Vec3d>(row, col);
				
				double dist = norm(pixel - color);

				if (dist <= range)
				{
					dst.at<Vec3b>(row, col) = Vec3b(255, 255, 255);

				}
				else
				{
					dst.at<Vec3b>(row, col) = Vec3b(0, 0, 0);
				}
			}
		}

		_dst.assign(dst);


	}
}
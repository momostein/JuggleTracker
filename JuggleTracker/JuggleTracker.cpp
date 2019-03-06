#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;
int main()
{
	Mat img = imread("resources\\lena.jpg");
	if (img.empty())
	{
		cout << "Could not open or find the image" << endl;
		cin.get();
		return -1;
	}

	namedWindow("image", WINDOW_NORMAL);
	imshow("image", img);
	waitKey(0);
	return 0;
}
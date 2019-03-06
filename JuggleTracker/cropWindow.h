#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>

namespace cropwin
{
	using namespace std;
	using namespace cv;

	enum CropState { INIT, TL, BR };

	

	class CropWindow
	{
	private:
		string name;
		Rect region;
		Mat image;
		CropState state;

		static void onMouse(int event, int x, int y, int flags, void *userdata);
		void onMouse(int event, int x, int y);

	public:
		CropWindow(const string & _name, WindowFlags flags);
		void setImage(const Mat & im);
		void show();
		Mat getCropped();

	};
}
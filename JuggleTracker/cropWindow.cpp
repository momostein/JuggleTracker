#include "cropWindow.h"

namespace cropwin
{
	void CropWindow::onMouse(int event, int x, int y, int flags, void * userdata)
	{
		if (userdata == NULL)
		{
			cout << "Error: userdata is nullpointer!" << endl;
			return;
		}

		CropWindow* cropWindow = reinterpret_cast<CropWindow*>(userdata);
		cropWindow->onMouse(event, x, y);
	}

	void CropWindow::onMouse(int event, int x, int y)
	{
		if (event != EVENT_LBUTTONDOWN)
			return;
		
		if (state == INIT)
			return;

		if (state == TL)
		{
			region = Rect(Point(x, y), region.br());
			state = BR;
		}
		else if (state == BR)
		{
			region = Rect(region.tl(), Point(x,y));
			state = TL;
		}

		show();
	}

	CropWindow::CropWindow(const string & _name, WindowFlags flags) : name(_name)
	{
		namedWindow(name, flags);
		setMouseCallback(name, onMouse, this);
		state = INIT;
	}

	void CropWindow::setImage(const Mat & im)
	{
		if (!im.empty())
		{ 
			image = im;

			if (state == INIT)
			{
				region = Rect(Point(0, 0), Point(image.size[1], image.size[0]));
				state = TL;
			}

			show();
		}
	}

	void CropWindow::show()
	{
		if (!image.empty())
		{

		}
		Mat draw = image.clone();
		rectangle(draw, region, Scalar(0, 255, 0), 2);
		cv::imshow(name, draw);
	}
	Mat CropWindow::getCropped()
	{
		return image(region).clone();
	}
}
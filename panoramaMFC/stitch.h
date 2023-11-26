#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;

namespace stitch {
	Mat stitch_two_image(Mat original_image, Mat object_image);
	bool compareX(const Point& p1, const Point& p2);
}
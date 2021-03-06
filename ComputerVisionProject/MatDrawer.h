#pragma once

#include "Enums.h"
#include "OpenCVHeaders.h"
#include "Util.h"

using namespace cv;

class MatDrawer
{
public:
	void DrawLabelToContour(Mat& im, const std::string label, std::vector<cv::Point>& contour, Scalar color = Util::Colors::White);
	void DrawPointsTo(Mat input, std::vector<Point> points, ColorChannelIdx colorChannelIdx);
	void DrawRectanglePointsTo(Mat input, RotatedRect rectangle, ColorChannelIdx colorChannelIdx);
};

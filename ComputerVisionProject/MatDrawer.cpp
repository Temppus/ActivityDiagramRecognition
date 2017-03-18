#include "MatDrawer.h"


void MatDrawer::DrawPointsTo(Mat input, std::vector<Point> points, ColorChannelIdx colorChannelIdx)
{
	for (int i = 0; i < points.size(); i++)
	{
		input.at<Vec3b>(points[i].y, points[i].x)[colorChannelIdx] = 255;
	}
}

void MatDrawer::DrawRectanglePointsTo(Mat input, RotatedRect rectangle, ColorChannelIdx colorChannelIdx)
{
	// read points
	cv::Point2f rect_points[4];
	rectangle.points(rect_points);

	// create integer points
	std::vector<Point> intPoints;

	// fill them
	for (int i = 0; i < 4; i++)
	{
		intPoints.push_back(Point((int)rect_points[i].x, (int)rect_points[i].y));
	}

	// draw
	DrawPointsTo(input, intPoints, colorChannelIdx);
}

void MatDrawer::DrawLabelToContour(Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	cv::Rect r = cv::boundingRect(contour);

	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255, 255, 255), CV_FILLED);
	cv::putText(im, label, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
}
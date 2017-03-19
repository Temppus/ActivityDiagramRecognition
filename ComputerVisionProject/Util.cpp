#include "Util.h"

const Scalar Util::Colors::Red = Scalar(0, 0, 255);
const Scalar Util::Colors::Green = Scalar(0, 255, 0);
const Scalar Util::Colors::Blue = Scalar(255, 0, 0);
const Scalar Util::Colors::White = Scalar(255, 255, 255);

RNG Util::Random::rng = RNG(12345);

/*
	Taken and modified from http://stackoverflow.com/questions/34237253/detect-centre-and-angle-of-rectangles-in-an-image-using-opencv
*/
int Util::Angles::CalculateRectangleAngle(RotatedRect rectangle)
{
	cv::Point2f rect_points[4];
	rectangle.points(rect_points);

	// choose the longer edge of the rotated rect to compute the angle
	cv::Point2f edge1 = cv::Vec2f(rect_points[1].x, rect_points[1].y) - cv::Vec2f(rect_points[0].x, rect_points[0].y);
	cv::Point2f edge2 = cv::Vec2f(rect_points[2].x, rect_points[2].y) - cv::Vec2f(rect_points[1].x, rect_points[1].y);

	cv::Point2f usedEdge = edge1;

	if (cv::norm(edge2) > cv::norm(edge1))
		usedEdge = edge2;

	cv::Point2f reference = cv::Vec2f(1, 0); // horizontal edge

	double angle = 180.0 / CV_PI * acos((reference.x * usedEdge.x + reference.y * usedEdge.y) / (cv::norm(reference) * cv::norm(usedEdge)));
	return static_cast<int>(angle);
}

Scalar Util::Random::RandomColor()
{
	int b = rng.uniform(0, 255);
	int g = rng.uniform(0, 255);
	int r = rng.uniform(0, 255);

	return Scalar(b, g, r);
}
#pragma once

#include <vector>
#include "OpenCVHeaders.h"

namespace activity
{
	class ActivityElement;
	class LineElement;
}

enum LinePoint
{
	LINE_POINT_START,
	LINE_POINT_END,
};

struct ClosestInfo
{
	activity::ActivityElement* ele;
	cv::Point closestLinePoint;
	double distance;

	ClosestInfo()
	{
		ele = nullptr;
		distance = 0.0f;
		closestLinePoint = cv::Point(0, 0);
	}

	ClosestInfo(activity::ActivityElement* ele, cv::Point closestLinePoint, double distance)
	{
		this->ele = ele;
		this->closestLinePoint = closestLinePoint;
		this->distance = distance;
	}
};

class ActivityFactory
{
public:
	static void CreateActivityElements(cv::Mat cannyEdgeMat, cv::Mat grayMat, cv::Mat dillMat, std::vector<activity::ActivityElement*>& activityElements);
private:
	static void FindConnectingElements(std::vector<activity::ActivityElement*>& activityElements, std::vector<cv::Point> lineContour,
										ClosestInfo &from, ClosestInfo &to, cv::Mat lineMat, int lineIndex);
};

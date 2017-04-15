#pragma once

#include <vector>

namespace activity
{
	class ActivityElement;
	class LineElement;
}

namespace cv
{
	class Mat;
}

enum LinePoint
{
	LINE_POINT_START,
	LINE_POINT_END,
};

class ActivityFactory
{
public:
	static void CreateActivityElements(cv::Mat cannyEdgeMat, cv::Mat grayMat, cv::Mat dillMat, std::vector<activity::ActivityElement*>& activityElements);
private:
	static activity::ActivityElement* FindConnectingElement(std::vector<activity::ActivityElement*>& activityElements, activity::LineElement * line, LinePoint lineTypePoint, double &minDistance);
};

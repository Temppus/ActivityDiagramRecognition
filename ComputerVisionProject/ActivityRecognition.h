#pragma once

#include "OpenCVHeaders.h"

using namespace cv;

class ActivityRecognition
{
public:
	void RenderActionAndDecisionElements(const Mat input, Mat dst, std::vector<std::vector<cv::Point>>& actionContours, std::vector<std::vector<cv::Point>>& decisionContours);
	void RenderInitialNode(const Mat input, Mat dst, std::vector<cv::Point>& initialNodeContour);
	void RenderFinalNodes(const Mat input, Mat dst, std::vector<std::vector<cv::Point>>& finalNodeContours);
	Mat RenderConnectingLines(const Mat inputDrawingMat, const Mat inputDillatedMat, std::vector<std::vector<cv::Point>>& lineContours);

	ActivityRecognition() {}
};

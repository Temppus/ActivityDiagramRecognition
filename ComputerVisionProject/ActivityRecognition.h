#pragma once

#include "OpenCVHeaders.h"
#include "CvWrapperTypeDefs.h"

using namespace cv;
using namespace std;

class ActivityRecognition
{
public:

	void FindActionElements(const Mat cannyMat, Mat &dstMat, Contours& actionContours, double rectAreaDiffPct = 0.15);
	void FindDecisionElements(const Mat cannyMat, Mat &dstMat, Contours& decisionContours, double rectAreaDiffPct = 0.15, int MaxAngleOffset = 10);
	void FindConnectingLines(const Mat drawingMatWithoutLines, const Mat cannyMat, Mat &dstMat, Contours& lineContours);


	void RenderActionAndDecisionElements(const Mat input, Mat dst, std::vector<std::vector<cv::Point>>& actionContours, std::vector<std::vector<cv::Point>>& decisionContours);
	void RenderInitialNode(const Mat dillinput, Mat dst, std::vector<cv::Point>& initialNodeContour);
	void RenderFinalNodes(const Mat input, Mat dst, std::vector<std::vector<cv::Point>>& finalNodeContours);
	Mat RenderLineArrows(const Mat inputDrawingMat, const Mat inputDillatedMat);

	Mat RenderConnectingLines(const Mat inputDrawingMat, const Mat inputDillatedMat, std::vector<std::vector<cv::Point>>& lineContours, Mat &arrowLinesMat);

	ActivityRecognition() {}
};

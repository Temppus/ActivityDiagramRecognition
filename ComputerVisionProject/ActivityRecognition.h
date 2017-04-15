#pragma once

#include "OpenCVHeaders.h"
#include "CvWrapperTypeDefs.h"

using namespace cv;
using namespace std;

class ActivityRecognition
{
public:

	void FindInitialNode(const Mat dillinput, Mat dst, Contour& initialNodeContour, Vec3i& circleVec);
	void FindFinalNodes(const Mat input, Mat dst, Contours& finalNodeContours, std::vector<Vec3i>& circlesVec);
	void FindActionElements(const Mat cannyMat, Mat &dstMat, Contours& actionContours, std::vector<Rect>& actionRectangles, double rectAreaDiffPct = 0.15);
	void FindDecisionElements(const Mat cannyMat, Mat &dstMat, Contours& decisionContours, std::vector<RotatedRect>& decisionRectangles, double rectAreaDiffPct = 0.15, int MaxAngleOffset = 10);
	void FindConnectingLines(const Mat drawingMatWithoutLines, const Mat cannyMat, Mat &dstMat, Contours& lineContours);

	Mat RenderLineArrows(const Mat inputDrawingMat, const Mat inputDillatedMat);

	ActivityRecognition() {}
};

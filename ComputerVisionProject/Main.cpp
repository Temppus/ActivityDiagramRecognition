#include "OpenCVHeaders.h"
#include "Enums.h"
#include "ActivityRecognition.h"
#include "MatTransformer.h"

#include <map>
#include <set>
#include <iostream>

typedef std::vector<std::vector<cv::Point>> Contours;
typedef std::vector<cv::Point> Contour;
 
using namespace cv;
using namespace std;

int main(int, char** argv)
{
	Mat srcMat = imread("C:\\Users\\Ivan\\Desktop\\test2.jpg", IMREAD_COLOR);

	if (srcMat.empty())
		return -1;

	MatTransformer matTransformer;

	Mat grayMat = matTransformer.ToGray(srcMat, true);
	imshow("gray", grayMat);

	Mat edgesMat = matTransformer.ToEdges(grayMat);
	imshow("canny edge", edgesMat);

	Mat linesMat = matTransformer.ToGray(matTransformer.ToHoughLinesP(edgesMat, 10, 30, 20), true);
	imshow("hough lines", linesMat);

	// Create empty color image for drawing
	Mat drawingMat = Mat::zeros(srcMat.size(), CV_8UC3);

	Mat dillMat = matTransformer.FillGaps(edgesMat);

	imshow("dillatated", dillMat);

	ActivityRecognition shapeRecognition;

	Contour initialNodeContour;
	shapeRecognition.RenderInitialNode(dillMat, drawingMat, initialNodeContour);

	Contours actionContours;
	Contours decisionContours;
	shapeRecognition.RenderActionAndDecisionElements(linesMat, drawingMat, actionContours, decisionContours);

	Contours finalNodeContours;
	shapeRecognition.RenderFinalNodes(grayMat, drawingMat, finalNodeContours);

	Contours lineContours;
	Mat binaryConnectingLinesMat = shapeRecognition.RenderConnectingLines(drawingMat, dillMat, lineContours);

	imshow("drawing", drawingMat);

	imshow("Connecting lines", binaryConnectingLinesMat);

	waitKey(0);
	return EXIT_SUCCESS;
}


#include "OpenCVHeaders.h"
#include "Enums.h"
#include "ShapeRecognition.h"
#include "MatTransformer.h"

#include <map>
#include <set>
#include <iostream>

using namespace cv;
using namespace std;

int main(int, char** argv)
{
	Mat srcMat = imread("C:\\Users\\Ivan\\Desktop\\test2.jpg", IMREAD_COLOR);

	if (srcMat.empty())
		return -1;

	MatTransformer mt;

	Mat grayMat = mt.ToGray(srcMat, true);
	//imshow("gray", grayMat);

	Mat edgesMat = mt.ToEdges(grayMat);
	//imshow("canny edge", edgesMat);

	Mat linesMat = mt.ToGray(mt.ToHoughLinesP(edgesMat, 10, 30, 20), true);
	imshow("hough lines", linesMat);

	// Create empty color image for drawing
	Mat drawingMat = Mat::zeros(srcMat.size(), CV_8UC3);

	Mat dillMat = mt.FillGaps(edgesMat);

	imshow("dillatated", dillMat);

	ShapeRecognition::renderStartingCircle(dillMat, drawingMat);
	ShapeRecognition::renderRectangles(linesMat, drawingMat);
	ShapeRecognition::renderFinalCircle(grayMat, drawingMat);


	Mat binaryConnectingLinesMat = ShapeRecognition::transformToConnectingLinesMat(drawingMat, dillMat);



	imshow("Connecting lines", binaryConnectingLinesMat);


	waitKey(0);
	return EXIT_SUCCESS;
}


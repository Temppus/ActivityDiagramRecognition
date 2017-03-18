#pragma once

#include "OpenCVHeaders.h"

using namespace cv;

class ShapeRecognition
{
public:
	static void renderRectangles(const Mat input, Mat dst);
	static void renderStartingCircle(const Mat input, Mat dst);
	static void renderFinalCircle(const Mat input, Mat dst);

	static Mat ShapeRecognition::transformToConnectingLinesMat(const Mat inputDrawingMat, const Mat inputDillatedMat);

private:
	ShapeRecognition() {}
};

#pragma once

#include "Enums.h"
#include "OpenCVHeaders.h"

using namespace cv;

class MatTransformer
{
public:
	MatTransformer() {}
	Mat ToGray(const Mat input, bool blurBefore);
	Mat ToEdges(const Mat input);
	Mat ToHoughLinesP(const Mat input, int threshold, double minLineLength = 0, double maxLineGap = 0);
	Mat FillGaps(const Mat input, int erodeTimes = 3);
};

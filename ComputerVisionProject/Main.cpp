#include "OpenCVHeaders.h"
#include "Enums.h"
#include "ActivityRecognition.h"
#include "MatTransformer.h"
#include "ActivityFactory.h"
#include "MatDrawer.h"
#include "ActivityElementHeaders.h"

#include <map>
#include <set>
#include <iostream>

using namespace cv;
using namespace std;
using namespace activity;

int main(int, char** argv)
{
	MatTransformer matTransformer;
	ActivityRecognition activityRecognition;

	// Read image
	Mat srcMat = imread("C:\\Users\\Ivan\\Desktop\\test6.jpg", IMREAD_COLOR);

	if (srcMat.empty())
		return -1;

	// Transform to grayscale
	Mat grayMat = matTransformer.ToGray(srcMat, true);
	imshow("gray", grayMat);

	// Create edge mat
	Mat cannyEdgeMat = matTransformer.ToEdges(grayMat);
	imshow("canny", cannyEdgeMat);

	// Close gaps in lines
	Mat dillMat = matTransformer.FillGaps(cannyEdgeMat);
	imshow("dillCanny", dillMat);

	std::vector<ActivityElement*> activityElements;
	ActivityFactory::CreateActivityElements(cannyEdgeMat, grayMat, dillMat, activityElements);

	Mat finalMat = Mat::zeros(srcMat.size(), CV_8UC3);

	// Draw recognized activity elements
	for (auto &activityEle : activityElements)
	{
		activityEle->Draw(finalMat);
		activityEle->DrawLabel(finalMat);
	}

	// Draw final result
	cv::imshow("finalMat", finalMat);

	// Free memory
	for (auto activityEle : activityElements)
	{
		delete activityEle;
	}

	waitKey();
	return EXIT_SUCCESS;
}


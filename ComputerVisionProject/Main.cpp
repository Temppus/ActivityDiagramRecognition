#include "OpenCVHeaders.h"
#include "Enums.h"
#include "ActivityRecognition.h"
#include "MatTransformer.h"
#include "ActivityFactory.h"
#include "MatDrawer.h"

#include "..\ComputerVisionProject\AcitivityElement.h"

#include <map>
#include <set>
#include <iostream>

using namespace cv;
using namespace std;
using namespace activity;

int main(int, char** argv)
{
	Mat srcMat = imread("C:\\Users\\Ivan\\Desktop\\test2.jpg", IMREAD_COLOR);

	if (srcMat.empty())
		return -1;

	MatDrawer drawer;
	MatTransformer matTransformer;

	Mat grayMat = matTransformer.ToGray(srcMat, true);
	imshow("gray", grayMat);

	Mat edgesMat = matTransformer.ToEdges(grayMat);
	//imshow("canny edge", edgesMat);

	Mat linesMat = matTransformer.ToGray(matTransformer.ToHoughLinesP(edgesMat, 10, 30, 20), true);
	//imshow("hough lines", linesMat);

	// Create empty color image for drawing
	Mat drawingMat = Mat::zeros(srcMat.size(), CV_8UC3);

	Mat dillMat = matTransformer.FillGaps(edgesMat);

	//imshow("dillatated", dillMat);

	std::vector<ActivityElement*> activityElements;
	ActivityFactory::CreateActivityElements(grayMat, drawingMat, dillMat, linesMat, activityElements);

	Mat finalMat = Mat::zeros(srcMat.size(), CV_8UC3);
	for (auto &activityEle : activityElements)
	{
		drawContours(finalMat, Contours{ activityEle->GetContour() }, 0, Util::Colors::Green, 2, 8);

		if (activityEle->GetTypeId() == ACTIVITY_TYPE_ID_LINE)
		{
			drawer.DrawLabelToContour(finalMat, ((LineElement*)activityEle)->GetFromElement()->GetName() + " , " + ((LineElement*)activityEle)->GetToElement()->GetName(), activityEle->GetContour());
		}
		else
			drawer.DrawLabelToContour(finalMat, activityEle->GetName(), activityEle->GetContour());
	}

	imshow("final", finalMat);

	waitKey(0);
	return EXIT_SUCCESS;
}


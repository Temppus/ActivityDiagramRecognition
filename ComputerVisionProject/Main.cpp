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
	MatDrawer drawer;

	Mat srcMat = imread("C:\\Users\\Ivan\\Desktop\\test2.jpg", IMREAD_COLOR);

	if (srcMat.empty())
		return -1;

	/*VideoCapture cap(0);
	if (!cap.isOpened())
		return -1;*/

	/*while (true)
	{
		Mat srcMat;
		cap.retrieve(srcMat); // get a new frame from camera

		imshow("aaa", srcMat);
		waitKey(5);
*/
		Mat grayMat = matTransformer.ToGray(srcMat, true);
		//imshow("gray", grayMat);

		Mat cannyEdgeMat = matTransformer.ToEdges(grayMat);
		Mat dillMat = matTransformer.FillGaps(cannyEdgeMat);
		//imshow("canny edge", cannyEdgeMat);

		std::vector<ActivityElement*> activityElements;
		ActivityFactory::CreateActivityElements(cannyEdgeMat, grayMat, dillMat, activityElements);

		Mat finalMat = Mat::zeros(srcMat.size(), CV_8UC3);
		for (auto &activityEle : activityElements)
		{
			drawContours(finalMat, Contours{ activityEle->GetContour() }, 0, Util::Colors::Green, 2, 8);

			if (activityEle->GetTypeId() == ACTIVITY_TYPE_ID_LINE)
			{
				const ActivityElement * fromEle = ((LineElement*)activityEle)->GetFromElement();
				const ActivityElement * toEle = ((LineElement*)activityEle)->GetToElement();
				string lineName = fromEle->GetName() + string(" , ") + toEle->GetName();

				drawer.DrawLabelToContour(finalMat, lineName, activityEle->GetContour());
			}
			else
				drawer.DrawLabelToContour(finalMat, activityEle->GetName(), activityEle->GetContour());
		}

		imshow("finalMat", finalMat);
		waitKey();

		for (auto activityEle : activityElements)
		{
			delete activityEle;
		}

	waitKey();
	return EXIT_SUCCESS;
}


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
			if (activityEle->GetTypeId() == ACTIVITY_TYPE_ID_LINE)
				activityEle->Draw(finalMat);

			//activityEle->DrawLabel(finalMat);
		}

		imshow("finalMat", finalMat);



		/// Detector parameters
		/*int blockSize = 15;
		int apertureSize = 8;
		double k = 0.1;

		Mat dst, dst_norm, dst_norm_scaled;
		dst = Mat::zeros(finalMat.size(), CV_32FC1);
		/// Detecting corners
		cornerHarris(matTransformer.ToGray(finalMat, true), dst, blockSize, apertureSize, k, BORDER_DEFAULT);

		/// Normalizing
		normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
		convertScaleAbs(dst_norm, dst_norm_scaled);

		/// Drawing a circle around corners
		for (int j = 0; j < dst_norm.rows; j++)
		{
			for (int i = 0; i < dst_norm.cols; i++)
			{
				if ((int)dst_norm.at<float>(j, i) > 160)
				{
					circle(finalMat, Point(i, j), 2, Util::Colors::Blue, CV_FILLED, 8, 0);
				}
			}
		}

		imshow("corners", finalMat);
		*/
		for (auto activityEle : activityElements)
		{
			delete activityEle;
		}
//	}
	waitKey();
	return EXIT_SUCCESS;
}


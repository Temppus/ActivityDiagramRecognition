#pragma once

#include "OpenCVHeaders.h"
#include "CvWrapperTypeDefs.h"

using namespace cv;

namespace Util
{
	class Angles
	{
	public:
		static int CalculateRectangleAngle(RotatedRect rectangle);
		static double RadiansToDegrees(double radians);
	};
}

namespace Util
{
	class Colors
	{
	public:
		static const Scalar Red;
		static const Scalar Green;
		static const Scalar Blue;
		static const Scalar White;
	};
}

namespace Util
{
	class Random
	{
	public:
		static Scalar RandomColor();
	public:
		static RNG rng;
	};
}

namespace Util
{
	class Image
	{
	public:
		static Mat FindCornerPoints(Mat &srcMat, std::vector<Point> &cornerPoints, int blockSize = 15, int apertureSize = 5, double k = 0.05, int threshValue = 150, int circleRadius = 8)
		{
			if (srcMat.channels() != 1)
				cvtColor(srcMat, srcMat, CV_BGR2GRAY);

			Mat cornerMat = Mat::zeros(srcMat.size(), srcMat.type());

			Mat dst, dst_norm, dst_norm_scaled;
			dst = Mat::zeros(srcMat.size(), srcMat.type());

			// Detect corners
			cornerHarris(srcMat, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

			// Normalizing
			normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
			convertScaleAbs(dst_norm, dst_norm_scaled);

			for (int j = 0; j < dst_norm.rows; j++)
			{
				for (int i = 0; i < dst_norm.cols; i++)
				{
					if ((int)dst_norm.at<float>(j, i) > threshValue)
					{
						cornerPoints.push_back(Point(i, j));
						circle(cornerMat, Point(i, j), circleRadius, Util::Colors::White, CV_FILLED, 8, 0);
					}
				}
			}

			/*imshow("corner", cornerMat);
			waitKey();
			*/
			return cornerMat;
		}

		static void FindCentroidsForContours(Mat &srcMat, std::vector<Point> &centroidPoints, int circleRadius = 8)
		{
			Contours contours;
			findContours(srcMat, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

			for (int i = 0; i < contours.size(); i++)
			{
				Moments mm = moments(contours[i], false);

				double x = mm.m10 / mm.m00;
				double y = mm.m01 / mm.m00;
				
				Point centroid = Point((int)x, (int)y);
				centroidPoints.push_back(centroid);		
			}
		}

		static void DrawArrowedLine(Mat &dstMat, Point pt1, Point pt2, const Scalar& color, int thickness, int line_type, double tipSize)
		{
			line(dstMat, pt1, pt2, color, thickness, line_type);

			const double angle = atan2((double)pt1.y - pt2.y, (double)pt1.x - pt2.x);

			Point p(cvRound(pt2.x + tipSize * cos(angle + CV_PI / 4)),
				cvRound(pt2.y + tipSize * sin(angle + CV_PI / 4)));

			line(dstMat, p, pt2, color, thickness, line_type);

			p.x = cvRound(pt2.x + tipSize * cos(angle - CV_PI / 4));
			p.y = cvRound(pt2.y + tipSize * sin(angle - CV_PI / 4));
			line(dstMat, p, pt2, color, thickness, line_type);
		}
	};
}




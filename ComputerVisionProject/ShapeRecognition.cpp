#include "Util.h"
#include "ActivityRecognition.h"
#include "Enums.h"
#include "MatTransformer.h"
#include "MatDrawer.h"

#include <iostream>

using namespace std;

void ActivityRecognition::RenderActionAndDecisionElements(const Mat input, Mat dst, std::vector<std::vector<cv::Point>>& actionContours, std::vector<std::vector<cv::Point>>& decisionContours)
{
	// Contour structures
	vector<vector<cv::Point>> contours;
	vector<Vec4i> hierarchy;
	vector<cv::Point> approxPolyPoints;

	cv::findContours(input.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	float imageArea = static_cast<float>(input.rows * input.cols);

	for (int i = 0; i < contours.size(); i++)
	{
		double lengthCoef = 0.1;
		double arcLength = cv::arcLength(cv::Mat(contours[i]), true) * lengthCoef;

		// Approximate contour with accuracy proportional to the contour perimeter
		cv::approxPolyDP(cv::Mat(contours[i]), approxPolyPoints, arcLength, true);

		if (approxPolyPoints.size() == 4)
		{
			// Ignore contours which has child contours (not sure why find contours returns multiple contours for same object)
			if (hierarchy[i][HIERARCHY_FIRT_CHILD] != HIERARCHY_VALUE_NONE)
				continue;

			RotatedRect rotatedRect = cv::minAreaRect(approxPolyPoints);
			//Helpers::DrawRectanglePointsTo(drawing, rotatedRect, CHANNEL_GREEN_IDX);

			float rectArea = rotatedRect.size.width * rotatedRect.size.height;
			float rectAreaPct = rectArea / imageArea * 100.0f;

			if (rectAreaPct < 0.4f || rectAreaPct > 5.0f)
				continue;

			drawContours(dst, contours, i, Util::Colors::Green , 5, 8, hierarchy, 0);

			//Helpers::DrawPointsTo(drawing, approxPolyPoints, CHANNEL_RED_IDX);

			int angle = Util::Angles::CalculateRectangleAngle(rotatedRect);
			string stringAngle = to_string(angle);

			string shapeName;

			if (angle < 10 || angle > 170)
			{
				actionContours.push_back(contours[i]);
				shapeName = "ACTION";
			}
			else
			{
				decisionContours.push_back(contours[i]);
				shapeName = "DECISION";
			}

			MatDrawer md;
			md.DrawLabelToContour(dst, shapeName, contours[i]);

			cout << angle << endl;
			cout << endl;
		}
	}
}

void ActivityRecognition::RenderFinalNodes(const Mat input, Mat dst, std::vector<std::vector<cv::Point>>& finalNodeContours)
{
	vector<Vec3f> circles;

	HoughCircles(input, circles, HOUGH_GRADIENT, 1, 10,
		100, 30, 1, 30 // change the last two parameters
					   // (min_radius & max_radius) to detect larger circles
	);

	// Create empty mat for contour detection
	Mat emptyMat = Mat::zeros(input.size(), CV_8UC1);

	for (size_t i = 0; i < circles.size(); i++)
	{
		Vec3i c = circles[i];

		circle(dst, Point(c[0], c[1]), c[2], Util::Colors::Green, 3, LINE_AA);
		circle(dst, Point(c[0], c[1]), 2, Util::Colors::Green, 3, LINE_AA);

		// draw to contour mat
		circle(emptyMat, Point(c[0], c[1]), c[2], Util::Colors::White, 3, LINE_AA);

		cv::putText(dst, "Final node", Point(c[0], c[1]), cv::FONT_HERSHEY_SIMPLEX, 0.4, Util::Colors::White, 2, 8);
	}

	// Extract contours of circles
	vector<vector<cv::Point>> contours;
	cv::findContours(input.clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++)
	{
		finalNodeContours.push_back(contours[i]);
	}
}

void ActivityRecognition::RenderInitialNode(const Mat input, Mat dst, std::vector<cv::Point>& initialNodeContour)
{
	// Contour structures
	vector<vector<cv::Point>> contours;
	vector<Vec4i> hierarchy;
	vector<cv::Point> approxPolyPoints;

	cv::findContours(input.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++)
	{
		double lengthCoef = 0.03;
		double arcLength = cv::arcLength(cv::Mat(contours[i]), true) * lengthCoef;

		// Approximate contour with accuracy proportional to the contour perimeter
		cv::approxPolyDP(cv::Mat(contours[i]), approxPolyPoints, arcLength, true);

		if (approxPolyPoints.size() > 6)
		{
			//minEnclosingCircle((Mat)approxPolyPoints[i], center[i], radius[i]);
			cv::Rect r = cv::boundingRect(contours[i]);
			cv::Point pt(r.x + ((r.width) / 2), r.y + ((r.height) / 2));

			circle(dst, pt, 10, Util::Colors::Green, 2);
			MatDrawer md;
			md.DrawLabelToContour(dst, "Initial node", contours[i]);

			initialNodeContour = contours[i];
			break;
		}
	}
}

Mat ActivityRecognition::RenderConnectingLines(const Mat inputDrawingMat, const Mat inputDillatedMat, std::vector<std::vector<cv::Point>>& lineContours)
{
	Mat drawingMat = inputDrawingMat.clone();
	Mat dillMat = inputDillatedMat.clone();

	MatTransformer mt;

	Mat colorDrawing = mt.ToGray(drawingMat, true).clone();
	colorDrawing = mt.FillGaps(colorDrawing, 0);

	// Convert to binary image
	cv::Mat binaryDrawing(colorDrawing.size(), colorDrawing.type());
	cv::threshold(colorDrawing, binaryDrawing, 100, 255, cv::THRESH_BINARY);

	Mat andMat;
	bitwise_and(binaryDrawing, dillMat, andMat);

	//imshow("AND 1", andMat);

	Mat andNot;
	bitwise_not(andMat, andNot);

	//imshow("AND NOT", andNot);

	Mat finalMat;
	bitwise_and(andNot, dillMat, finalMat);

	//imshow("AND 2", finalMat);

	erode(finalMat, finalMat, getStructuringElement(MORPH_RECT, Size(5, 1)), Point(-1, -1), 1);
	erode(finalMat, finalMat, getStructuringElement(MORPH_RECT, Size(1, 5)), Point(-1, -1), 1);

	//imshow("ERODE", finalMat);

	// Extract lines
	std::vector<Vec4i> linesVec;
	HoughLinesP(finalMat, linesVec, 1, CV_PI / 180, 10, 50, 0);

	Mat linesImage = Mat::zeros(drawingMat.size(), CV_8UC3);

	// Draw them
	for (size_t i = 0; i < linesVec.size(); i++)
	{
		line(linesImage, Point(linesVec[i][0], linesVec[i][1]),
			Point(linesVec[i][2], linesVec[i][3]), Util::Colors::White, 3, 8);
	}

	// Transform to contours to reduce overlapping lines
	Mat connectingLinesMat = Mat::zeros(drawingMat.size(), CV_8UC1);
	vector<vector<cv::Point> > contours;
	vector<Vec4i> hierarchy;
	vector<cv::Point> approxPolyPoints;

	cv::findContours(mt.ToGray(linesImage, true), contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	// Draw them
	for (int i = 0; i < contours.size(); i++)
	{
		double lengthCoef = 0.1;
		double arcLength = cv::arcLength(cv::Mat(contours[i]), true) * lengthCoef;

		cv::approxPolyDP(cv::Mat(contours[i]), approxPolyPoints, arcLength, false);
		drawContours(connectingLinesMat, contours, i, Util::Colors::White, CV_FILLED, LINE_8, hierarchy, 0);

		lineContours.push_back(contours[i]);
	}

	return connectingLinesMat;
}

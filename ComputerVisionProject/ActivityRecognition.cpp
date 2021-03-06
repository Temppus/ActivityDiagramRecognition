#include "Util.h"
#include "ActivityRecognition.h"
#include "Enums.h"
#include "MatTransformer.h"
#include "MatDrawer.h"
#include <iostream>

void ActivityRecognition::FindActionElements(const Mat cannyMat, Mat &dstMat, Contours& actionContours, std::vector<Rect>& actionRectangles, double rectAreaDiffPct)
{
	//MatDrawer md;
	MatTransformer matTransformer;
	Mat dillEdgeMat = matTransformer.FillGaps(cannyMat);

	Contours contours;
	Hierarchy hierarchy;

	//imshow("dillEdgeMat", dillEdgeMat);

	findContours(dillEdgeMat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	if (dstMat.rows == 0 && dstMat.cols == 0)
		dstMat = Mat::zeros(dillEdgeMat.size(), CV_8UC3);

	for (int i = 0; i < contours.size(); i++)
	{
		Rect boundingRect = cv::boundingRect(Mat(contours[i]));

		double rawArea = cv::contourArea(contours[i]);
		double rectArea = boundingRect.area();
		double areasMaxDiff = rectArea * rectAreaDiffPct;

		double areasDiff = std::abs(rawArea - rectArea);

		if (areasDiff <= areasMaxDiff)
		{
			//rectangle(dstMat, boundingRect, Util::Colors::Green, 4);

			actionRectangles.push_back(boundingRect);
			actionContours.push_back(contours[i]);
			drawContours(dstMat, contours, i, Util::Colors::White, 2);
			//md.DrawLabelToContour(dstMat, "ACTION", contours[i]);
		}
	}

	//imshow("actions", dstMat);
	//waitKey();
}

void ActivityRecognition::FindDecisionElements(const Mat cannyMat, Mat &dstMat, Contours& decisionContours, std::vector<RotatedRect>& decisionRectangles, double rectAreaDiffPct, int MaxAngleOffset)
{
	//MatDrawer md;
	MatTransformer matTransformer;
	Mat dillEdgeMat = matTransformer.FillGaps(cannyMat);

	Contours contours;
	Hierarchy hierarchy;

	//imshow("dillEdgeMat", dillEdgeMat);

	findContours(dillEdgeMat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	if (dstMat.rows == 0 && dstMat.cols == 0)
		dstMat = Mat::zeros(dillEdgeMat.size(), CV_8UC3);

	Mat decisionMat = Mat::zeros(cannyMat.size(), CV_8UC3);

	for (int i = 0; i < contours.size(); i++)
	{
		RotatedRect rotatedRect = cv::minAreaRect(contours[i]);
		Rect boundingRotatedRect = rotatedRect.boundingRect();

		double rawArea = cv::contourArea(contours[i]);
		double rectArea = rotatedRect.size.height * rotatedRect.size.width;
		double areasMaxDiff = rectArea * rectAreaDiffPct;

		double areasDiff = std::abs(rawArea - rectArea);

		if (areasDiff <= areasMaxDiff)
		{
			int angle = Util::Angles::CalculateRectangleAngle(rotatedRect);

			// Action element
			if (angle < (0 + MaxAngleOffset) || angle >(180 - MaxAngleOffset))
				continue;

			// Decision element
			decisionRectangles.push_back(rotatedRect);
			decisionContours.push_back(contours[i]);
			drawContours(dstMat, contours, i, Util::Colors::White, 2);

			/*Point2f pts[4];
			rotatedRect.points(pts);
			Contour rectContour{ pts[0], pts[1], pts[2], pts[3] };

			drawContours(decisionMat, contours, i, Util::Colors::White, 2);
			drawContours(decisionMat, Contours{ rectContour }, 0, Util::Colors::Green, 2);
			//md.DrawLabelToContour(dstMat, "DECISION", contours[i]);
			*/
		}
	}

	//imshow("decisions", decisionMat);
	//waitKey();
}

void ActivityRecognition::FindFinalNodes(const Mat input, Mat dst, Contours& finalNodeContours, std::vector<Vec3i>& circlesVec)
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

		circle(dst, Point(c[0], c[1]), c[2], Util::Colors::White, 3, LINE_AA);
		circlesVec.push_back(c);

		// draw to contour mat
		circle(emptyMat, Point(c[0], c[1]), c[2], Util::Colors::White, 3, LINE_AA);
	}

	// Extract contours of circles
	vector<vector<cv::Point>> contours;
	vector<Vec4i> hierarchy;
	cv::findContours(emptyMat.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++)
	{
		if (hierarchy[i][HIERARCHY_FIRT_CHILD] == HIERARCHY_VALUE_NONE)
			finalNodeContours.push_back(contours[i]);
	}
}

void ActivityRecognition::FindInitialNode(const Mat dillinput, Mat dst, Contour& initialNodeContour, Vec3i& circleVec, double circleAreaDiffPct)
{
	// Contour structures
	vector<vector<cv::Point>> contours;
	vector<Vec4i> hierarchy;
	vector<cv::Point> approxPolyPoints;

	cv::findContours(dillinput.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++)
	{
		if (hierarchy[i][HIERARCHY_FIRT_CHILD] != HIERARCHY_VALUE_NONE)
			continue;

		Point2f centrePoint;
		float radius;
		minEnclosingCircle(contours[i], centrePoint, radius);

		double rawArea = cv::contourArea(contours[i]);
		double circleArea = CV_PI * radius * radius;
		double areasMaxDiff = circleArea * circleAreaDiffPct;

		double areasDiff = std::abs(rawArea - circleArea);

		if (areasDiff <= areasMaxDiff)
		{
			initialNodeContour = contours[i];
			circleVec = Vec3i((int)centrePoint.x, (int)centrePoint.y, (int)radius);
			break;
		}
	}
}

void ActivityRecognition::FindConnectingLines(const Mat drawingMatWithoutLines, const Mat cannyMat, Mat &dstMat, Contours& lineContours)
{
	MatTransformer mt;

	Mat dillMat = mt.FillGaps(cannyMat);
	Mat grayDillDrawingMat = mt.FillGaps(mt.ToGray(drawingMatWithoutLines, true), 0);

	// Convert to binary image
	cv::Mat binaryDrawing(grayDillDrawingMat.size(), grayDillDrawingMat.type());
	cv::threshold(grayDillDrawingMat, binaryDrawing, 100, 255, cv::THRESH_BINARY);

	Mat andMat;
	bitwise_and(binaryDrawing, dillMat, andMat);

	//imshow("AND 1", andMat);

	Mat andNot;
	bitwise_not(andMat, andNot);

	//imshow("AND NOT", andNot);

	Mat finalMat;
	bitwise_and(andNot, dillMat, finalMat);


	erode(finalMat, finalMat, getStructuringElement(MORPH_RECT, Size(5, 1)));
	erode(finalMat, finalMat, getStructuringElement(MORPH_RECT, Size(1, 5)));

	//imshow("AND NOT", finalMat);
	//waitKey();

	// Extract lines
	std::vector<Vec4i> linesVec;
	HoughLinesP(finalMat, linesVec, 1, CV_PI / 180, 10, 10, 5);

	Mat linesImage = Mat::zeros(drawingMatWithoutLines.size(), CV_8UC3);

	// Draw them
	for (size_t i = 0; i < linesVec.size(); i++)
	{
		line(linesImage, Point(linesVec[i][0], linesVec[i][1]),
			Point(linesVec[i][2], linesVec[i][3]), Util::Colors::White, 1, LINE_8);
	}

	//erode(linesImage, linesImage, getStructuringElement(MORPH_RECT, Size(2, 2)), Point(-1,1), 2);

	//imshow("houg lines", linesImage);

	// Transform to contours to reduce overlapping lines
	Mat connectingLinesMat = Mat::zeros(drawingMatWithoutLines.size(), CV_8UC1);
	Contours contours;
	Hierarchy hierarchy;

	cv::findContours(mt.ToGray(linesImage, true), contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	if (dstMat.rows == 0 && dstMat.cols == 0)
		dstMat = Mat::zeros(drawingMatWithoutLines.size(), CV_8UC3);

	// Draw them
	for (int i = 0; i < contours.size(); i++)
	{
		drawContours(dstMat, contours, i, Util::Colors::White, CV_FILLED, LINE_8, hierarchy, 0);
		lineContours.push_back(contours[i]);
	}

	//imshow("find lines", dstMat);
	//waitKey();
}

Mat ActivityRecognition::RenderLineArrows(const Mat inputDrawingMat, const Mat inputDillatedMat)
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

	return finalMat;
}

#pragma once

#include "..\ComputerVisionProject\ActivityElementHeaders.h"
#include "Util.h"

using namespace activity;

enum LineEnding
{
	LINE_ENDING_START,
	LINE_ENDING_END,
};

class ActivityFactory
{
private:
	Contours _lineContours;
	Mat _linesMat;
	unsigned int _rows;
	unsigned int _columns;
	unsigned int _idCounter;

public:
	ActivityFactory(unsigned int rows, unsigned int columns, Contours lineContours)
	{
		_idCounter = 1;
		_rows = rows;
		_columns = columns;
		_linesMat = Mat::zeros(_rows, _columns, CV_8UC1);

		_lineContours = lineContours;

		for (int i = 0; i < lineContours.size(); i++)
		{
			drawContours(_linesMat, lineContours, i, Util::Colors::White, CV_FILLED, LINE_8);
		}
	}

	static void CreateActivityElements(Mat grayMat, Mat drawingMat, Mat dillMat, Mat linesMat, std::vector<ActivityElement*>& activityElements)
	{
		int rows = drawingMat.rows;
		int cols = drawingMat.cols;

		ActivityRecognition shapeRecognition;

		Contour initialNodeContour;
		shapeRecognition.RenderInitialNode(dillMat, drawingMat, initialNodeContour);

		activityElements.push_back(new InitialNodelement(1, initialNodeContour));

		Contours actionContours;
		Contours decisionContours;
		shapeRecognition.RenderActionAndDecisionElements(linesMat, drawingMat, actionContours, decisionContours);

		for (int actionIndex = 0; actionIndex < actionContours.size(); actionIndex++)
		{
			activityElements.push_back(new ActionElement(actionIndex + 1, actionContours[actionIndex]));
		}

		for (int decisionIndex = 0; decisionIndex < decisionContours.size(); decisionIndex++)
		{
			activityElements.push_back(new DecisionElement(decisionIndex + 1, decisionContours[decisionIndex]));
		}

		Contours finalNodeContours;
		shapeRecognition.RenderFinalNodes(grayMat, drawingMat, finalNodeContours);

		for (int finalNodeIndex = 0; finalNodeIndex < finalNodeContours.size(); finalNodeIndex++)
		{
			activityElements.push_back(new FinalNodeElement(finalNodeIndex + 1, finalNodeContours[finalNodeIndex]));
		}

		Contours lineContours;
		Mat binaryConnectingLinesMat = shapeRecognition.RenderConnectingLines(drawingMat, dillMat, lineContours);

		for (int lineIndex = 0; lineIndex < lineContours.size(); lineIndex++)
		{
			Contour lineContour = lineContours[lineIndex];
			LineElement * line = new LineElement(lineIndex + 1, lineContour, nullptr, nullptr);

			for (auto &activityEle : activityElements)
			{
				// Create drawing mat for acttivity element contour
				Mat activityEleMat = Mat::zeros(rows, cols, CV_8UC1);
				drawContours(activityEleMat, Contours{ activityEle->GetContour() }, 0, Util::Colors::White, 5, LINE_8);

				// Find intersecting element from start of line
				Mat lineFromMat = Mat::zeros(rows, cols, CV_8UC1);
				RenderEndpointCirclesForLine(lineContour, lineFromMat, LINE_ENDING_START);

				Mat andFromMat = Mat::zeros(rows, cols, CV_8UC1);
				bitwise_and(activityEleMat, lineFromMat, andFromMat);

				if (countNonZero(andFromMat) > 0)
				{
					line->SetFromElement(activityEle);
				}

				// Find intersecting element from end of line
				Mat lineToMat = Mat::zeros(rows, cols, CV_8UC1);
				RenderEndpointCirclesForLine(lineContour, lineToMat, LINE_ENDING_END);

				Mat andToMat = Mat::zeros(rows, cols, CV_8UC1);
				bitwise_and(activityEleMat, lineToMat, andToMat);

				if (countNonZero(andToMat) > 0)
				{
					line->SetToElement(activityEle);
				}
			}

			if (!line->IsEmpty())
				activityElements.push_back(line);
			else
				delete line;
		}
	}

	private:
	static void RenderEndpointCirclesForLine(Contour lineContour, Mat outPutMat, LineEnding lineEnding)
	{
		Contour approxLineContour;

		double arcLengthCoef = cv::arcLength(lineContour, true) * 0.2;
		cv::approxPolyDP(lineContour, approxLineContour, arcLengthCoef, true);

		Point point = lineEnding == LINE_ENDING_START ? approxLineContour.front() : approxLineContour.back();
		int lineLength = static_cast<int>(arcLength(approxLineContour, false));

		int radius = static_cast<int>((outPutMat.rows + outPutMat.cols) * 0.015f);
		radius = radius > (lineLength / 2) ? (lineLength / 2) : radius;

		drawContours(outPutMat, Contours{ approxLineContour }, 0, Util::Colors::White, CV_FILLED, LINE_8);
		circle(outPutMat, point, radius, Util::Colors::White, 2, LINE_8, 0);
	}
};

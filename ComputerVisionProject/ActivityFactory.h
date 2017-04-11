#pragma once

#include "..\ComputerVisionProject\ActivityElementHeaders.h"
#include "Util.h"
#include <limits>
#include <tuple>

#include "boolinq.h"

using namespace activity;

enum LinePoint
{
	LINE_POINT_START,
	LINE_POINT_END,
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

		std::multiset<std::tuple<double, LineElement*>> lineMaxDistances;

		for (int lineIndex = 0; lineIndex < lineContours.size(); lineIndex++)
		{
			Contour lineContour = lineContours[lineIndex];
			LineElement * line = new LineElement(lineIndex + 1, lineContour, nullptr, nullptr);

			double minValueStart;
			double minValueEnd;
			line->SetFromElement(FindConnectingElement(activityElements, line, LINE_POINT_START, minValueStart));
			line->SetToElement(FindConnectingElement(activityElements, line, LINE_POINT_END, minValueEnd));

			lineMaxDistances.insert(make_tuple(std::max(minValueStart, minValueEnd), line));
		}

		std::vector<std::tuple<double, LineElement*>> resultVec(lineMaxDistances.begin(), lineMaxDistances.end());
		double thresholdPct = 3.0;
		int ignoreFromIndex = -1;

		auto filteredLines = boolinq::from(resultVec).where_i([resultVec, thresholdPct, &ignoreFromIndex](std::tuple<double, LineElement*> currentTuple, int i)
		{
			if (i == ignoreFromIndex)
			{
				delete std::get<1>(currentTuple);
				return false;
			}

			if (i == 0 || i == resultVec.size() - 1)
				return true;

			if (std::get<0>(currentTuple) * thresholdPct < (std::get<0>(resultVec[i + 1])))
			{
				ignoreFromIndex = i + 1;
			}
			return true;
		}).select([&activityElements](std::tuple<double, LineElement*> tuple) 
		{ 
			auto lineEle = std::get<1>(tuple);
			activityElements.push_back(lineEle);
			return lineEle;
		}).toVector();
	}

private:
	static ActivityElement* FindConnectingElement(std::vector<ActivityElement*>& activityElements, LineElement * line, LinePoint lineTypePoint, double &minDistance)
	{
		if (activityElements.empty())
			return nullptr;

		Contour approxLineContour;

		double arcLengthCoef = cv::arcLength(line->GetContour(), true) * 0.2;
		cv::approxPolyDP(line->GetContour(), approxLineContour, arcLengthCoef, true);

		Point linePoint = lineTypePoint == LINE_POINT_START ? approxLineContour.front() : approxLineContour.back();

		size_t minDistanceIndex;
		minDistance = std::numeric_limits<double>::max();

		std::vector<double> distances;

		for (int i = 0; i < activityElements.size(); i++)
		{
			auto activityEle = activityElements[i];

			if (activityEle->GetTypeId() == ACTIVITY_TYPE_ID_LINE)
				continue;

			double currentDist = std::abs(pointPolygonTest(activityEle->GetContour(), linePoint, true));

			distances.push_back(currentDist);

			if (currentDist < minDistance)
			{
				minDistance = currentDist;
				minDistanceIndex = i;
			}
		}

		return activityElements[minDistanceIndex];
	}

	/*static void RenderEndpointCirclesForLine(Contour lineContour, Mat outPutMat, LineEnding lineEnding)
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

		imshow("EEE", outPutMat);
		waitKey(200);
	}*/


};

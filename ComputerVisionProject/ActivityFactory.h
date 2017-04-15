#pragma once

#include <vector>

namespace activity
{
	class ActivityElement;
	class LineElement;
}

namespace cv
{
	class Mat;
}

enum LinePoint
{
	LINE_POINT_START,
	LINE_POINT_END,
};

class ActivityFactory
{
public:
	static void CreateActivityElements(cv::Mat cannyEdgeMat, cv::Mat grayMat, cv::Mat dillMat, std::vector<activity::ActivityElement*>& activityElements);
private:
	static activity::ActivityElement* FindConnectingElement(std::vector<activity::ActivityElement*>& activityElements, activity::LineElement * line, LinePoint lineTypePoint, double &minDistance);

	/*static void CreateActivityElements(Mat grayMat, Mat drawingMat, Mat dillMat, Mat linesMat, std::vector<ActivityElement*>& activityElements)
	{
		int rows = drawingMat.rows;
		int cols = drawingMat.cols;

		ActivityRecognition activityRecognition;

		Contour initialNodeContour;
		activityRecognition.RenderInitialNode(dillMat, drawingMat, initialNodeContour);

		activityElements.push_back(new InitialNodelement(1, initialNodeContour));

		Contours actionContours;
		Contours decisionContours;
		activityRecognition.RenderActionAndDecisionElements(linesMat, drawingMat, actionContours, decisionContours);

		for (int actionIndex = 0; actionIndex < actionContours.size(); actionIndex++)
		{
			activityElements.push_back(new ActionElement(actionIndex + 1, actionContours[actionIndex]));
		}

		for (int decisionIndex = 0; decisionIndex < decisionContours.size(); decisionIndex++)
		{
			activityElements.push_back(new DecisionElement(decisionIndex + 1, decisionContours[decisionIndex]));
		}

		Contours finalNodeContours;
		activityRecognition.RenderFinalNodes(grayMat, drawingMat, finalNodeContours);

		for (int finalNodeIndex = 0; finalNodeIndex < finalNodeContours.size(); finalNodeIndex++)
		{
			activityElements.push_back(new FinalNodeElement(finalNodeIndex + 1, finalNodeContours[finalNodeIndex]));
		}

		Contours lineContours;
		Mat arrowLinesMat;
		Mat binaryConnectingLinesMat = activityRecognition.RenderConnectingLines(drawingMat, dillMat, lineContours, arrowLinesMat);

		Mat andMat;
		bitwise_and(arrowLinesMat, binaryConnectingLinesMat, andMat);

		Mat andNot;
		bitwise_not(andMat, andNot);

		Mat arrowsMat;
		bitwise_and(andNot, arrowLinesMat, arrowsMat);

		dilate(arrowsMat, arrowsMat, getStructuringElement(MORPH_RECT, Size(5, 5)), Point(-1, -1), 3);
		erode(arrowsMat, arrowsMat, getStructuringElement(MORPH_RECT, Size(5, 5)), Point(-1, -1), 3);

		imshow("arrows", arrowsMat);

		std::multiset<std::tuple<double, LineElement*>> lineInfoDistances;

		for (int lineIndex = 0; lineIndex < lineContours.size(); lineIndex++)
		{
			Contour lineContour = lineContours[lineIndex];
			LineElement * line = new LineElement(lineIndex + 1, lineContour, nullptr, nullptr);

			double minValueStart;
			double minValueEnd;

			auto connectingEle1 = FindConnectingElement(activityElements, line, LINE_POINT_START, minValueStart);
			auto connectingEle2 = FindConnectingElement(activityElements, line, LINE_POINT_END, minValueEnd);



			line->SetFromElement(connectingEle1);
			line->SetToElement(connectingEle2);

			lineInfoDistances.insert(make_tuple(std::max(minValueStart, minValueEnd), line));
		}

		std::vector<std::tuple<double, LineElement*>> resultVec(lineInfoDistances.begin(), lineInfoDistances.end());
		double thresholdPct = 3.0;
		int ignoreFromIndex = -1;

		auto filteredLines = boolinq::from(resultVec).where_i([resultVec, thresholdPct, &ignoreFromIndex](std::tuple<double, LineElement*> currentTuple, int i)
		{
			if (i == ignoreFromIndex)
			{
				delete std::get<1>(currentTuple);
				std::get<1>(currentTuple) = nullptr;
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
	*/

private:
	/*static ActivityElement* FindConnectingElement(std::vector<ActivityElement*>& activityElements, LineElement * line, LinePoint lineTypePoint, double &minDistance)
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
	*/

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

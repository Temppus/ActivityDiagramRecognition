#include "ActivityFactory.h"

#include "Util.h"
#include "boolinq.h"
#include "ActivityRecognition.h"
#include "ActivityElementHeaders.h"
#include <limits>
#include <tuple>

using namespace std;
using namespace cv;
using namespace activity;

void ActivityFactory::CreateActivityElements(Mat cannyEdgeMat, Mat grayMat, Mat dillMat, vector<ActivityElement*>& activityElements)
{
	ActivityRecognition activityRecognition;
	Mat drawingMat;

	// ACTION NODES
	Contours actionContours;
	std::vector<Rect> actionRectangles;
	activityRecognition.FindActionElements(cannyEdgeMat, drawingMat, actionContours, actionRectangles);

	for (int actionIndex = 0; actionIndex < actionContours.size(); actionIndex++)
	{
		activityElements.push_back(new ActionElement(actionIndex + 1, actionRectangles[actionIndex], actionContours[actionIndex]));
	}

	// DECISION NODES
	Contours decisionContours;
	std::vector<RotatedRect> decisionRectangles;
	activityRecognition.FindDecisionElements(cannyEdgeMat, drawingMat, decisionContours, decisionRectangles);

	for (int decisionIndex = 0; decisionIndex < decisionContours.size(); decisionIndex++)
	{
		activityElements.push_back(new DecisionElement(decisionIndex + 1, decisionRectangles[decisionIndex], decisionContours[decisionIndex]));
	}

	// INITIAL NODE
	Contour initialNodeContour;
	Vec3i centreVec;
	activityRecognition.FindInitialNode(dillMat, drawingMat, initialNodeContour, centreVec);

	activityElements.push_back(new InitialNodelement(1, centreVec, nullptr, initialNodeContour));

	// FINAL NODES
	Contours finalNodeContours;
	std::vector<Vec3i> circlesVec;
	activityRecognition.FindFinalNodes(grayMat, drawingMat, finalNodeContours, circlesVec);

	for (int finalNodeIndex = 0; finalNodeIndex < finalNodeContours.size(); finalNodeIndex++)
	{
		activityElements.push_back(new FinalNodeElement(finalNodeIndex + 1, circlesVec[finalNodeIndex], nullptr, finalNodeContours[finalNodeIndex]));
	}

	// LINE NODES
	Contours lineContours;
	activityRecognition.FindConnectingLines(drawingMat, cannyEdgeMat, drawingMat, lineContours);

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

	//imshow("drawing", drawingMat);
	//waitKey();
}


ActivityElement* ActivityFactory::FindConnectingElement(std::vector<ActivityElement*>& activityElements, LineElement * line, LinePoint lineTypePoint, double &minDistance)
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
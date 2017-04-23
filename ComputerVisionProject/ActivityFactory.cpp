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

	if (initialNodeContour.size() != 0)
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

		Mat lineTempMat = Mat::zeros(cannyEdgeMat.size(), CV_8UC1);
		drawContours(lineTempMat, Contours{ lineContour }, 0, Util::Colors::White, CV_FILLED, LINE_8);

		ClosestInfo startingInfo;
		ClosestInfo endingInfo;

		FindConnectingElements(activityElements, lineContour, startingInfo, endingInfo, lineTempMat, lineIndex + 1);

		if (startingInfo.ele == nullptr || endingInfo.ele == nullptr)
			continue;

		Point startingPoint = startingInfo.closestLinePoint;
		Point endingPoint = endingInfo.closestLinePoint;

		double linePartLength = arcLength(lineContour, false) * 0.1;
		double startCircleRadius = arcLength(lineContour, false) * 0.12;
		double endCircleRadius = arcLength(lineContour, false) * 0.12;

		startCircleRadius = startCircleRadius > linePartLength ? linePartLength : startCircleRadius;
		endCircleRadius = endCircleRadius > linePartLength ? linePartLength : endCircleRadius;

		std::vector<Point> cornerPoints;
		Mat cornerMat = Util::Image::FindCornerPoints(lineTempMat, cornerPoints);

		std::vector<Point> centroidsPoints;
		double circleRadius = 8.0;
		Util::Image::FindCentroidsForContours(cornerMat, centroidsPoints, (int)circleRadius);

		auto filteredCornerPoints = boolinq::from(centroidsPoints).where([startingPoint, startCircleRadius, endingPoint, endCircleRadius, circleRadius](Point centroidP)
		{
			if (norm(centroidP - startingPoint) < circleRadius)
				return false;

			if (norm(centroidP - endingPoint) < circleRadius)
				return false;

			return true;
		}).toVector();


		std::vector<cv::Point> linePoints;

		// Draw direct line
		if (filteredCornerPoints.empty())
		{
			linePoints.push_back(startingPoint);
			linePoints.push_back(endingPoint);
		}
		// Draw line with corners
		else
		{
			linePoints.push_back(startingPoint);

			if (lineIndex == 21)
			{
				filteredCornerPoints.front();
			}

			filteredCornerPoints.insert(filteredCornerPoints.begin(), startingPoint);
			filteredCornerPoints.push_back(endingPoint);

			std::set<int> visitedSet;

			//line(dstMat, startPoint, endPoint, Util::Colors::Green, 2, LINE_AA);

			visitedSet.insert(0);
			visitedSet.insert((int)filteredCornerPoints.size() - 1);

			int searchIndex = 0;
			Point searchPoint = filteredCornerPoints[searchIndex];

			while (visitedSet.size() != filteredCornerPoints.size())
				//for (int i = 0; i < filteredCornerPoints.size(); i++)
			{
				/*if (i == filteredCornerPoints.size() - 1)
					break;*/

					//line(dstMat, filteredCornerPoints[i], filteredCornerPoints[i +1], Util::Colors::White, 2, LINE_AA);

				Point currentPoint = searchPoint; /*filteredCornerPoints[i];*/
				Point closestPoint(cannyEdgeMat.cols + 1, cannyEdgeMat.rows + 1);
				int closestIndex = -1;

				for (int j = 0; j < filteredCornerPoints.size(); j++)
				{
					if (searchIndex == j)
						continue;

					bool foundCloser = norm(currentPoint - filteredCornerPoints[j]) <= norm(currentPoint - closestPoint);

					if (foundCloser && visitedSet.count(j) == 0)
					{
						closestIndex = j;
						closestPoint = filteredCornerPoints[j];
					}
				}

				linePoints.push_back(closestPoint);
				searchPoint = closestPoint;
				searchIndex = closestIndex;

				visitedSet.insert(closestIndex);
			}

			linePoints.push_back(endingPoint);

			//line(dstMat, closestPoint, endPoint, Util::Colors::White, 2, LINE_AA);
		}

		if (startingInfo.ele != nullptr && endingInfo.ele != nullptr)
		{
			LineElement * line = new LineElement(lineIndex + 1, lineContour, linePoints, nullptr, nullptr);

			line->SetFromElement(startingInfo.ele);
			line->SetToElement(endingInfo.ele);

			//activityElements.push_back(line);

			lineInfoDistances.insert(make_tuple(startingInfo.distance + endingInfo.distance, line));
		}
	}

	std::vector<std::tuple<double, LineElement*>> resultVec(lineInfoDistances.begin(), lineInfoDistances.end());
	double thresholdPct = 4.0;
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


void ActivityFactory::FindConnectingElements(std::vector<activity::ActivityElement*>& activityElements, std::vector<cv::Point> lineContour,
	ClosestInfo &from, ClosestInfo &to, Mat lineMat, int lineIndex)
{
	std::vector<ClosestInfo> closestInfos;

	for (auto &linePoint : lineContour)
	{
		for (auto &activityEle : activityElements)
		{
			if (activityEle->GetTypeId() == ACTIVITY_TYPE_ID_LINE)
				continue;

			double dist = std::abs(pointPolygonTest(activityEle->GetContour(), linePoint, true));
			closestInfos.push_back(ClosestInfo(activityEle, linePoint, dist));
		}
	}

	std::sort(begin(closestInfos), end(closestInfos), [](ClosestInfo const &t1, ClosestInfo const &t2) {
		return t1.distance < t2.distance;
	});

	if (closestInfos.empty())
		return;

	ClosestInfo first = closestInfos[0];
	ClosestInfo second;

	for (auto &closestInfo : closestInfos)
	{
		if (closestInfo.ele->GetName() != first.ele->GetName())
		{
			second = closestInfo;
			break;
		}
	}

	if (second.ele == nullptr)
		return;

	Mat firstMask = Mat::zeros(lineMat.size(), CV_8UC1);
	circle(firstMask, first.closestLinePoint, 10, Util::Colors::White, CV_FILLED);

	Mat firstAndMask;
	bitwise_and(firstMask, lineMat, firstAndMask);

	Mat secondMask = Mat::zeros(lineMat.size(), CV_8UC1);
	circle(secondMask, second.closestLinePoint, 10, Util::Colors::White, CV_FILLED);

	Mat secondAndMask;
	bitwise_and(secondMask, lineMat, secondAndMask);

	if (countNonZero(firstAndMask) < countNonZero(secondAndMask))
	{
		from = first;
		to = second;
	}
	else
	{
		to = first;
		from = second;
	}
}
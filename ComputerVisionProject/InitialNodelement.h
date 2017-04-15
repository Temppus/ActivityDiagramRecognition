#pragma once

#include "AcitivityElement.h"

namespace activity
{
	class InitialNodelement : public ActivityElement
	{
	private:
		ActivityElement* _nextFlowElement;
		Vec3i _centerVec;
		Contour _contour;

	public:
		InitialNodelement(int id, Vec3i centerVec, ActivityElement *nextFlowElement, Contour contour) : ActivityElement(ACTIVITY_TYPE_ID_INITIAL_NODE, "I", id, contour)
			, _nextFlowElement(nextFlowElement)
		{
			_contour = contour;
			_centerVec = centerVec;
		}

		void Draw(Mat &dstMat) override
		{
			circle(dstMat, Point(_centerVec[0], _centerVec[1]), _centerVec[2], Util::Colors::White, 2, LINE_AA);
		}

		void SetNextActivityElement(ActivityElement* nextActivityEle)
		{
			_nextFlowElement = nextActivityEle;
		}
	};
}
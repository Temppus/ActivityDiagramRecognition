#pragma once

#include "ActionElement.h"

namespace activity
{
	class FinalNodeElement : public ActivityElement
	{
	private:
		ActivityElement* _previousFlowElement;
		Vec3i _centerVec;

	public:
		FinalNodeElement(int id, Vec3i centerVec, ActivityElement *previousFlowElement, Contour contour) : ActivityElement(ACTIVITY_TYPE_ID_FINAL_NODE, "F", id, contour)
			, _previousFlowElement(previousFlowElement)
		{
			_centerVec = centerVec;
		}

		void Draw(Mat &dstMat) override
		{
			circle(dstMat, Point(_centerVec[0], _centerVec[1]), _centerVec[2], Util::Colors::White, 2, LINE_AA);
		}

		void SetPreviousActivityElement(ActivityElement* previousActivityEle)
		{
			_previousFlowElement = previousActivityEle;
		}
	};
}
#pragma once

#include "AcitivityElement.h"

namespace activity
{
	class InitialNodelement : public ActivityElement
	{
	private:
		ActivityElement* _nextFlowElement;

	public:
		InitialNodelement(int id, Contour contour, ActivityElement *nextFlowElement = nullptr) : ActivityElement(ACTIVITY_TYPE_ID_INITIAL_NODE, "INITIAL NODE", id, contour)
			, _nextFlowElement(nextFlowElement)
		{
		}

		void SetNextActivityElement(ActivityElement* nextActivityEle)
		{
			_nextFlowElement = nextActivityEle;
		}
	};
}
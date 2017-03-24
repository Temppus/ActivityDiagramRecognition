#pragma once

#include "AcitivityElement.h"

namespace activity
{
	class InitialNodelement : public ActivityElement
	{
	private:
		ActivityElement* _nextFlowElement;

	public:
		InitialNodelement(TypeId typeId, string name, Contour contour, ActivityElement *nextFlowElement = nullptr) : ActivityElement(typeId, name, contour)
			, _nextFlowElement(nextFlowElement)
		{
		}

		void SetNextActivityElement(ActivityElement* nextActivityEle)
		{
			_nextFlowElement = nextActivityEle;
		}
	};
}
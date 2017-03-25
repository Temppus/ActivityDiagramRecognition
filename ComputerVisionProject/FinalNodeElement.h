#pragma once

#include "ActionElement.h"

namespace activity
{
	class FinalNodeElement : public ActivityElement
	{
	private:
		ActivityElement* _previousFlowElement;
	public:
		FinalNodeElement(int id, Contour contour, ActivityElement *previousFlowElement = nullptr) : ActivityElement(ACTIVITY_TYPE_ID_FINAL_NODE, "FINAL NODE", id, contour)
			, _previousFlowElement(previousFlowElement)
		{
		}

		void SetPreviousActivityElement(ActivityElement* previousActivityEle)
		{
			_previousFlowElement = previousActivityEle;
		}
	};
}
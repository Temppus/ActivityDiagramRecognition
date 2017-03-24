#pragma once

#include "ActionElement.h"

namespace activity
{
	class FinalNodeElement : public ActivityElement
	{
	private:
		ActivityElement* _previousFlowElement;
	public:
		FinalNodeElement(TypeId typeId, string name, Contour contour, ActivityElement *previousFlowElement = nullptr): ActivityElement(typeId, name, contour)
			, _previousFlowElement(previousFlowElement)
		{
		}

		void SetPreviousActivityElement(ActivityElement* previousActivityEle)
		{
			_previousFlowElement = previousActivityEle;
		}
	};
}
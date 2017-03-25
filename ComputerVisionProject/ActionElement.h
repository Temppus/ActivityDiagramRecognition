#pragma once

#include "AcitivityElement.h"

namespace activity
{
	class ActionElement : public ActivityElement
	{
	private:
		std::vector<ActivityElement*> _inputElements;
		ActivityElement* _outputElement;

	public:
		ActionElement(int id, Contour contour) : ActivityElement(ACTIVITY_TYPE_ID_ACTION, "ACTION", id, contour)
		{
		}

		void AddInputAcitivityElement(ActivityElement* ouputEle)
		{
			_inputElements.push_back(ouputEle);
		}

		void SetOutputActivityElement(ActivityElement* ouputEle)
		{
			_outputElement = ouputEle;
		}
	};
}

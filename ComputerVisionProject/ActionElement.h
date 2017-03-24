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
		ActionElement(TypeId typeId, string name, Contour contour) : ActivityElement(typeId, name, contour)
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

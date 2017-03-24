#pragma once

#include "AcitivityElement.h"

namespace activity
{
	class DecisionElement : public ActivityElement
	{
	private:
		ActivityElement* _positiveElement;
		ActivityElement* _negativeElement;

	public:
		DecisionElement(TypeId typeId, string name, Contour contour, ActivityElement *positiveElement = nullptr, ActivityElement *negativeElement = nullptr) : ActivityElement(typeId, name, contour)
			, _positiveElement(positiveElement), _negativeElement(negativeElement)
		{
		}

		void SetPositiveActivityElement(ActivityElement* positiveEle)
		{
			_positiveElement = positiveEle;
		}

		void SetNegativeActivityElement(ActivityElement* negativeEle)
		{
			_negativeElement = negativeEle;
		}
	};
}
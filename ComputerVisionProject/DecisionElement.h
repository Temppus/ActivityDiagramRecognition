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
		DecisionElement(int id, Contour contour, ActivityElement *positiveElement = nullptr, ActivityElement *negativeElement = nullptr)
			: ActivityElement(ACTIVITY_TYPE_ID_DECISION, "DECISION", id, contour)
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
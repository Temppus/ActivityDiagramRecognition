#pragma once

#include "AcitivityElement.h"

namespace activity
{
	class LineElement : public ActivityElement
	{
	private:
		ActivityElement* _fromElement;
		ActivityElement* _toElement;

	public:
		LineElement(int id, Contour contour, ActivityElement *fromElement = nullptr, ActivityElement *toElement = nullptr)
			: ActivityElement(ACTIVITY_TYPE_ID_LINE, "LINE", id, contour)
			, _fromElement(fromElement), _toElement(toElement)
		{
			;
		}

		void SetFromElement(ActivityElement* fromELe)
		{
			_fromElement = fromELe;
		}

		void SetToElement(ActivityElement* toEle)
		{
			_toElement = toEle;
		}

		const ActivityElement * const GetFromElement() const
		{
			return _fromElement;
		}

		const ActivityElement * const GetToElement() const
		{
			return _toElement;
		}
	};
}


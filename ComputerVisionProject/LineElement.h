#pragma once

#include "AcitivityElement.h"

namespace activity
{
	class LineElement : public ActivityElement
	{
	private:
		ActivityElement* _fromElement;
		ActivityElement* _toElement;
		Contour _lineContour;

	public:
		LineElement(int id, Contour lineContour, ActivityElement *fromElement = nullptr, ActivityElement *toElement = nullptr)
			: ActivityElement(ACTIVITY_TYPE_ID_LINE, "L", id, lineContour)
			, _fromElement(fromElement), _toElement(toElement)
		{
			_lineContour = lineContour;
		}

		void Draw(Mat &dstMat) override
		{
			drawContours(dstMat, Contours{ _lineContour }, 0, Util::Colors::White, CV_FILLED, LINE_8);
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


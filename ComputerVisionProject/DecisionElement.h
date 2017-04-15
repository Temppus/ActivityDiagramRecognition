#pragma once

#include "AcitivityElement.h"

namespace activity
{
	class DecisionElement : public ActivityElement
	{
	private:
		ActivityElement* _positiveElement;
		ActivityElement* _negativeElement;
		RotatedRect _rectangle;

	public:
		DecisionElement(int id, RotatedRect rectangle, Contour contour, ActivityElement *positiveElement = nullptr, ActivityElement *negativeElement = nullptr)
			: ActivityElement(ACTIVITY_TYPE_ID_DECISION, "D", id, contour)
			, _positiveElement(positiveElement), _negativeElement(negativeElement)
		{
			_rectangle = rectangle;
		}

		void Draw(Mat &dstMat) override
		{
			Point2f rect_points[4];
			_rectangle.points(rect_points);

			for (int i = 0; i < 4; i++)
				line(dstMat, rect_points[i], rect_points[(i + 1) % 4], Util::Colors::White, 2, LINE_8);
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
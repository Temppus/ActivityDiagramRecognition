#pragma once

#include "AcitivityElement.h"

namespace activity
{
	class ActionElement : public ActivityElement
	{
	private:
		std::vector<ActivityElement*> _inputElements;
		ActivityElement* _outputElement;
		Rect _rectangle;

	public:
		ActionElement(int id, Rect rectangle, Contour contour) : ActivityElement(ACTIVITY_TYPE_ID_ACTION, "A", id, contour)
		{
			_rectangle = rectangle;
		}

		void Draw(Mat &dstMat) override
		{
			rectangle(dstMat, _rectangle, Util::Colors::White, 2);
		}

		Contour GetSmoothContour(int blockParts = 0)
		{
			Contour smoothContour;

			// order matters !
			smoothContour.push_back(Point(_rectangle.tl().x, _rectangle.tl().y));
			smoothContour.push_back(Point(_rectangle.tl().x + _rectangle.width, _rectangle.tl().y));
			smoothContour.push_back(Point(_rectangle.br().x, _rectangle.br().y));
			smoothContour.push_back(Point(_rectangle.br().x - _rectangle.width, _rectangle.br().y));

			return smoothContour;
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

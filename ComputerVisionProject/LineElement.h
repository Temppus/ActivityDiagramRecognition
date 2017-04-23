#pragma once

#include "AcitivityElement.h"
#include <vector>
#include <tuple>
#include "boolinq.h"

namespace activity
{
	class LineElement : public ActivityElement
	{
	private:
		ActivityElement* _fromElement;
		ActivityElement* _toElement;
		Contour _lineContour;
		std::vector<cv::Point> _linePoints;

	public:
		LineElement(int id, Contour lineContour, Contour linePoints, ActivityElement *fromElement = nullptr, ActivityElement *toElement = nullptr)
			: ActivityElement(ACTIVITY_TYPE_ID_LINE, "L", id, lineContour)
			, _fromElement(fromElement), _toElement(toElement)
		{
			_lineContour = lineContour;
			_linePoints = linePoints;
		}

		void Draw(Mat &dstMat) override
		{
			//drawContours(dstMat, Contours{ _lineContour }, 0, Util::Colors::White, CV_FILLED, LINE_AA);

			if (GetId() == 25)
				GetId();

			circle(dstMat, _linePoints[0], 3, Util::Colors::Red, 2, LINE_AA);
			circle(dstMat, _linePoints[_linePoints.size() - 1], 3, Util::Colors::Red, 2, LINE_AA);

			for (int i = 0; i < _linePoints.size(); i++)
			{
				if (i == _linePoints.size() - 1)
					break;

				if (i == _linePoints.size() -2)
				{
					Util::Image::DrawArrowLine(dstMat, _linePoints[i], _linePoints[i + 1], Util::Colors::White, 2, LINE_AA, 15.0);
					//arrowedLine(dstMat, _linePoints[i], _linePoints[i + 1], Util::Colors::White, 2, LINE_AA, 0, );
				}
				else
				{
					line(dstMat, _linePoints[i], _linePoints[i + 1], Util::Colors::White, 2, LINE_AA);
				}

			}
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


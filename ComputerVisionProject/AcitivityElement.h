#pragma once

#include <string>
#include <vector>
#include "OpenCVHeaders.h"

using namespace std;
using namespace cv;

namespace activity
{
	typedef std::vector<cv::Point> Contour;
	typedef std::vector<std::vector<cv::Point>> Contours;

	enum TypeId
	{
		ACTIVITY_TYPE_ID_ACTION,
		ACTIVITY_TYPE_ID_DECISION,
		ACTIVITY_TYPE_ID_INITIAL_NODE,
		ACTIVITY_TYPE_ID_FINAL_NODE
	};

	class ActivityElement
	{
	private:
		TypeId _typeId;
		string _name;
		Contour _contour;
	public:
		ActivityElement(TypeId typeId, string name, Contour contour)
		{
			_typeId = typeId;
			_name = name;
			_contour = contour;
		}

		TypeId GetTypeId() { return _typeId; }
		string GetName() { return _name; }
		Contour GetContour() { return _contour; }
	};
}


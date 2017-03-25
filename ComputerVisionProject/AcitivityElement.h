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
		ACTIVITY_TYPE_ID_FINAL_NODE,
		ACTIVITY_TYPE_ID_LINE
	};

	class ActivityElement
	{
	private:
		TypeId _typeId;
		string _name;
		Contour _contour;

	public:
		ActivityElement(TypeId typeId, string name, int id, Contour contour)
		{
			_typeId = typeId;
			_name = name + " " + to_string(id);
			_contour = contour;
		}

		TypeId GetTypeId() const { return _typeId; }
		string GetName() const { return _name; }
		Contour GetContour() const { return _contour; }

	};
}


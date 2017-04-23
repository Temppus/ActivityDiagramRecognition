#pragma once

#include <string>
#include <vector>
#include "OpenCVHeaders.h"
#include "MatDrawer.h"

using namespace std;
using namespace cv;

namespace activity
{
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
		MatDrawer _md;
		int _id;

	public:
		ActivityElement(TypeId typeId, string name, int id, Contour contour)
		{
			_contour = contour;
			_typeId = typeId;
			_id = id;
			_name = name + " " + to_string(_id);
		}

		virtual void Draw(Mat &dstMat) = 0;

		void DrawLabel(Mat &dstMat)
		{
			_md.DrawLabelToContour(dstMat, _name, _contour);
		}

		int GetId() const { return _id; }
		Contour GetContour() { return _contour; }
		TypeId GetTypeId() const { return _typeId; }
		string GetName() const { return _name; }
	};
}


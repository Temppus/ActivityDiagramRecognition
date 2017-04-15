#pragma once

#include "OpenCVHeaders.h"

using namespace cv;

namespace Util
{
	class Angles
	{
	public:
		static int CalculateRectangleAngle(RotatedRect rectangle);
		static double RadiansToDegrees(double radians);
	};
}

namespace Util
{
	class Colors
	{
	public:
		static const Scalar Red;
		static const Scalar Green;
		static const Scalar Blue;
		static const Scalar White;
	};
}

namespace Util
{
	class Random
	{
	public:
		static Scalar RandomColor();
	public:
		static RNG rng;
	};
}




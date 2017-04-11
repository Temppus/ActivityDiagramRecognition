#pragma once

#include "OpenCVHeaders.h"

using namespace cv;

namespace Util
{
	class Angles
	{
	public:
		static int CalculateRectangleAngle(RotatedRect rectangle);
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

namespace Util
{
	class Containers
	{
	public:
		static std::vector<double> Util::Containers::FilterElementsByThresholdPct(const std::vector<double>& vec, double thresholdPctValue);
	};
}




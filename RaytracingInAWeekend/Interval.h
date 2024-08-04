#pragma once

#include "RT.h"

class Interval
{
public:
	double min, max;

	Interval()
		: min(-infinity), max(+infinity)
	{
	}

	Interval(double min, double max)
		: min(min), max(max)
	{
	}

	double Size()
	{
		return max - min;
	}

	bool Contains(double x) const
	{
		return min <= x && x <= max;
	}

	bool Surrounds(double x)
	{
		return min < x && x < max;
	}
};

const Interval empty = Interval(+infinity, -infinity);
const Interval universe = Interval(-infinity, +infinity);

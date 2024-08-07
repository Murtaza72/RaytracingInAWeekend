#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;
using std::fabs;


inline double Random()
{
	return rand() / (RAND_MAX + 1.0);
}

inline double Random(double min, double max)
{
	return min + (max - min) * Random();
}

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;


// Utility Functions
inline double degrees_to_radians(double degrees)
{
	return degrees * pi / 180.0;
}

#pragma once

#include <iostream>

#include "Interval.h"
#include "Vec3.h"

using Color = Vec3;

inline double LinearToGamma(double color)
{
	if (color < 0.0)
		return 0.0;

	return sqrt(color);
}

inline void WriteColor(std::ostream& out, const Color& pixelColor)
{
	auto r = pixelColor.x();
	auto g = pixelColor.y();
	auto b = pixelColor.z();

	// Linear to Gamma
	r = LinearToGamma(r);
	g = LinearToGamma(g);
	b = LinearToGamma(b);

	// Translate the [0,1] component values to the byte range [0,255].
	Interval intensity(0.000, 0.999);

	int rbyte = int(256 * intensity.Clamp(r));
	int gbyte = int(256 * intensity.Clamp(g));
	int bbyte = int(256 * intensity.Clamp(b));
	// Write out the pixel color components.
	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
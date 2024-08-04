#pragma once

#include "Hittable.h"

class Sphere : public Hittable
{
public:
	Sphere(const Point3& center, double radius)
		: m_Center(center), m_Radius(fmax(0, radius))
	{
	}

	bool Hit(const Ray& ray, Interval rayT, HitRecord& record) override;

private:
	Point3 m_Center;
	double m_Radius;
};
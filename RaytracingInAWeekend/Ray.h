#pragma once

#include "Vec3.h"

class Ray
{
public:
	Ray() {}

	Ray(const Point3& origin, const Vec3& direction) : m_Origin(origin), m_Dir(direction) {}

	const Point3& Origin() const { return m_Origin; }
	const Vec3& Direction() const { return m_Dir; }

	Point3 At(double t) const
	{
		return m_Origin + t * m_Dir;
	}

private:
	Point3 m_Origin;
	Vec3 m_Dir;
};
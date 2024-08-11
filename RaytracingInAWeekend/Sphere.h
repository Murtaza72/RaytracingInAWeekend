#pragma once

#include "Hittable.h"

class Sphere : public Hittable
{
public:
	Sphere(const Point3& center, double radius, shared_ptr<Material> material)
		: m_Center(center), m_Radius(fmax(0, radius)), m_Material(material)
	{
	}

	bool Hit(const Ray& ray, const Interval rayT, HitRecord& record) const override;

private:
	Point3 m_Center;
	double m_Radius;
	shared_ptr<Material> m_Material;
};
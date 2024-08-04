#pragma once

#include "Ray.h"
#include "Interval.h"

class HitRecord
{
public:
	Point3 point;
	Vec3 normal;
	double t;
	bool frontFace;

	void SetFrontFace(const Ray& ray, const Vec3& outwardNormal)
	{
		frontFace = dot(ray.Direction(), outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}

};

class Hittable
{
public:
	virtual ~Hittable() = default;

	virtual bool Hit(const Ray& ray, Interval rayT, HitRecord& record) = 0;
};
#pragma once

#include "Interval.h"
#include "Ray.h"

class Material;

class HitRecord
{
public:
	Point3 point;
	Vec3 normal;
	double t;		// scalar along the direction of ray
	bool frontFace;
	shared_ptr<Material> material;

public:
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

	virtual bool Hit(const Ray& ray, const Interval rayT, HitRecord& record) const = 0;
};
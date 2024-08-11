#pragma once

#include <vector>

#include "RT.h"

#include "Hittable.h"

class HittableList : public Hittable
{
public:
	std::vector<shared_ptr<Hittable>> objects;

	void Add(shared_ptr<Hittable> obj)
	{
		objects.push_back(obj);
	}

	bool Hit(const Ray& ray, Interval rayT, HitRecord& record) const override
	{
		HitRecord rec;
		bool hitAny = false;
		double closest = rayT.max;

		for (const auto& obj : objects)
		{
			if (obj->Hit(ray, Interval(rayT.min, closest), rec))
			{
				hitAny = true;
				closest = rec.t;
				record = rec;
			}
		}

		return hitAny;
	}
};
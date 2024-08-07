#include "Sphere.h"

bool Sphere::Hit(const Ray& ray, Interval rayT, HitRecord& record)
{
	Vec3 originToCenter = m_Center - ray.Origin();
	double a = ray.Direction().LengthSquared();
	double bHalf = dot(ray.Direction(), originToCenter);
	double c = originToCenter.LengthSquared() - (m_Radius * m_Radius);
	double discriminant = (bHalf * bHalf) - (a * c);

	if (discriminant < 0) return false;

	double sqrtDiscriminant = sqrt(discriminant);

	auto root = (bHalf - sqrtDiscriminant) / a;

	if (!rayT.Surrounds(root))
	{
		root = (bHalf + sqrtDiscriminant) / a;
		if (!rayT.Surrounds(root))
		{
			return false;
		}
	}

	record.t = root;
	record.point = ray.At(record.t);
	record.normal = (record.point - m_Center) / m_Radius;
	Vec3 outwardNormal = (record.point - originToCenter) / m_Radius;
	record.SetFrontFace(ray, outwardNormal);
	record.material = m_Material;

	return true;
}
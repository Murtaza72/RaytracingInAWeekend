#pragma once

#include "RT.h"

#include "Color.h"
#include "Ray.h"
#include "Hittable.h"

class HitRecord;

class Material
{
public:
	Material() = default;

	virtual bool Scattter(const Ray& ray, const HitRecord& rec, Color& attenuation, Ray& scattered) const
	{
		return false;
	}
};

class Lambertian : public Material
{
public:

	Lambertian(const Color& albedo) : m_Albedo(albedo) {}

	bool Scattter(const Ray& ray, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
	{
		Vec3 direction = rec.normal + RandomUnitVector();

		// if normal and random were complete opposite
		if (direction.NearZero())
			direction = rec.normal;

		scattered = Ray(rec.point, direction);
		attenuation = m_Albedo;

		return true;
	}

private:
	Color m_Albedo;
};

class Metal : public Material
{
public:

	Metal(const Color& albedo, double fuzz)
		: m_Albedo(albedo), m_Fuzz(fuzz < 1 ? fuzz : 1)
	{
	}

	bool Scattter(const Ray& ray, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
	{
		Vec3 reflected = Reflect(ray.Direction(), rec.normal);
		reflected = UnitVector(reflected) + (m_Fuzz * RandomUnitVector());

		scattered = Ray(rec.point, reflected);
		attenuation = m_Albedo;

		// avoid scattering below surface
		return (dot(scattered.Direction(), rec.normal) > 0);
	}

private:
	Color m_Albedo;
	double m_Fuzz;
};
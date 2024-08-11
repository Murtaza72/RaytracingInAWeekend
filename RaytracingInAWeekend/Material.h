#pragma once

#include "RT.h"

#include "Color.h"
#include "Ray.h"
#include "Hittable.h"

class HitRecord;

class Material
{
public:
	virtual ~Material() = default;

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

class Dielectric : public Material
{
public:

	Dielectric(double refractiveIndex)
		: m_RefractiveIndex(refractiveIndex)
	{
	}

	bool Scattter(const Ray& ray, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
	{
		attenuation = Color(1.0, 1.0, 1.0);

		double modifiedRI = rec.frontFace ? (1.0 / m_RefractiveIndex) : m_RefractiveIndex;

		Vec3 unitVec = UnitVector(ray.Direction());
		double cosTheta = fmin(dot(-unitVec, rec.normal), 1.0);
		double sinTheta = sqrt(1.0 - (cosTheta * cosTheta));

		bool cannotRefract = modifiedRI * sinTheta > 1.0;
		Vec3 direction;

		if (cannotRefract || Reflectance(cosTheta, modifiedRI) > Random())
		{
			direction = Reflect(unitVec, rec.normal);
		}
		else
		{
			direction = Refract(unitVec, rec.normal, modifiedRI);
		}

		scattered = Ray(rec.point, direction);

		return true;
	}

private:
	double m_RefractiveIndex;

	static double Reflectance(double cosine, double refractIndex)
	{
		auto r0 = (1 - refractIndex) / (1 + refractIndex);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
};
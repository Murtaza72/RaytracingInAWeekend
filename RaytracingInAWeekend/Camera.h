#pragma once

#include "Color.h"
#include "Hittable.h"
#include "HittableList.h"

class Camera
{
public:
	void Render(HittableList& world);

	int imageWidth = 1600;
	double aspectRatio = 1.0;

private:
	Color RayColor(const Ray& r, HittableList& world);

	void Initialize();

	int m_imageHeight;
	Point3 m_cameraCenter;
	Vec3 m_pixelDeltaU;
	Vec3 m_pixelDeltaV;
	Point3 m_pixel00Location;
};
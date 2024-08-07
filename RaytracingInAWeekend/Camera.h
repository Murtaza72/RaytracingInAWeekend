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
	int samplePerPixel = 100;
	int maxDepth = 32;

private:
	Color RayColor(const Ray& r, int depth, HittableList& world);
	Ray GetRay(int i, int j);
	Vec3 RandomSample();


	void Initialize();

	int m_imageHeight;
	Point3 m_cameraCenter;
	Vec3 m_pixelDeltaU;
	Vec3 m_pixelDeltaV;
	Point3 m_pixel00Location;
	double m_pixelSampleScale;
};
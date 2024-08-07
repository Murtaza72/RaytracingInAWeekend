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

	int m_ImageHeight;
	Point3 m_CameraCenter;
	Vec3 m_PixelDeltaU;
	Vec3 m_PixelDeltaV;
	Point3 m_Pixel00Location;
	double m_PixelSampleScale;
};
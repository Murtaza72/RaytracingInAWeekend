#pragma once

#include "Color.h"
#include "Hittable.h"
#include "HittableList.h"

class Camera
{
public:
	void Render(HittableList& world);

public:
	int imageWidth = 1600;
	double aspectRatio = 1.0;
	int samplePerPixel = 100;
	int maxDepth = 32;

	int vertFieldOfView = 45;
	Point3 lookFrom = Point3(0, 0, 0);
	Point3 lookAt = Point3(0, 0, -1);
	Vec3 viewUp = Vec3(0, 1, 0);

	double defocusAngle = 0.0;
	double focusDistance = 10;		// distance from camera to focus plane

private:
	void Initialize();

	Color RayColor(const Ray& r, int depth, HittableList& world);
	Ray GetRay(int i, int j);
	Vec3 RandomSample();
	Vec3 RandomDefocusDiskSample();

private:
	int m_ImageHeight;
	Point3 m_CameraCenter;
	Vec3 m_PixelDeltaU;
	Vec3 m_PixelDeltaV;
	Point3 m_Pixel00Location;
	double m_PixelSampleScale;
	Vec3 u, v, w;
	Vec3 m_DefocusHorizontal, m_DefocusVertical;
};
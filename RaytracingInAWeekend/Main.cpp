#include <iostream>

#include "Camera.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Ray.h"
#include "Sphere.h"
#include "Vec3.h"

int main()
{
	HittableList world;

	world.Add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
	world.Add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

	Camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 2000;
	cam.samplePerPixel = 10;
	cam.maxDepth = 32;

	cam.Render(world);

	return 0;
}
#include <iostream>

#include "Ray.h"
#include "Vec3.h"
#include "Sphere.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Camera.h"

int main()
{
	HittableList world;

	world.Add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
	world.Add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

	Camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 19200;

	cam.Render(world);

	return 0;
}
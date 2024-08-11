#include <iostream>

#include "Camera.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Ray.h"
#include "Sphere.h"
#include "Vec3.h"
#include "Material.h"

int main()
{
	HittableList world;

	auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
	auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
	auto material_left = make_shared<Dielectric>(1.5);
	auto materialBubble = make_shared<Dielectric>(1.0 / 1.33);
	auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

	world.Add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.Add(make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, material_center));
	world.Add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.Add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.3, materialBubble));
	world.Add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

	Camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 400;
	cam.samplePerPixel = 100;
	cam.maxDepth = 50;

	cam.vertFieldOfView = 45;
	cam.lookFrom = Point3(-2, 2, 1);
	cam.lookAt = Point3(0, 0, -1);
	cam.viewUp = Point3(0, 1, 0);

	cam.Render(world);

	return 0;
}
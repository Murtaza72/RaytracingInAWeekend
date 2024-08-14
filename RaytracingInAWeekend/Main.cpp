#include "RT.h"

#include "Camera.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Ray.h"
#include "Sphere.h"
#include "Vec3.h"
#include "Material.h"

#include <chrono>

void SampleRender()
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
	cam.imageWidth = 960;
	cam.samplePerPixel = 100;
	cam.maxDepth = 50;

	cam.vertFieldOfView = 45;
	cam.lookFrom = Point3(-2, 2, 1);
	cam.lookAt = Point3(0, 0, -1);
	cam.viewUp = Point3(0, 1, 0);

	cam.defocusAngle = 10;
	cam.focusDistance = 3.4;

	auto start = std::chrono::system_clock::now();

	cam.RenderMT(world);

	auto end = std::chrono::system_clock::now();
	double timeTaken = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::clog << "Time taken: " << timeTaken << " ms" << std::endl;
	std::cin.get();
}

void FinalRender()
{
	HittableList world;

	auto groundMaterial = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
	world.Add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMaterial));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			double chooseMat = Random();
			Point3 center(a + 0.9 * Random(), 0.2, b + 0.9 * Random());
			if ((center - Point3(4, 0.2, 0)).Length() > 0.9)
			{
				shared_ptr<Material> sphereMaterial;
				if (chooseMat < 0.8)
				{
					// diffuse
					Color albedo = RandomColor() * RandomColor();
					sphereMaterial = make_shared<Lambertian>(albedo);
					world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				else if (chooseMat < 0.95)
				{
					// metal
					Color albedo = RandomColor(0.5, 1);
					double fuzz = Random(0, 0.5);
					sphereMaterial = make_shared<Metal>(albedo, fuzz);
					world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				else
				{
					// glass
					sphereMaterial = make_shared<Dielectric>(1.5);
					world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
			}
		}
	}

	auto material1 = make_shared<Dielectric>(1.5);
	world.Add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
	world.Add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
	world.Add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

	Camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 1920;
	cam.samplePerPixel = 500;
	cam.maxDepth = 50;

	cam.vertFieldOfView = 20;
	cam.lookFrom = Point3(13, 2, 3);
	cam.lookAt = Point3(0, 0, 0);
	cam.viewUp = Vec3(0, 1, 0);

	cam.defocusAngle = 0.6;
	cam.focusDistance = 10.0;

	cam.Render(world);
}

int main()
{
	SampleRender();

	return 0;
}
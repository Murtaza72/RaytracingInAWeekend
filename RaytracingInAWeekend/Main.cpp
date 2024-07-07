#include <iostream>

#include "Color.h"
#include "Ray.h"
#include "Vec3.h"

Color RayColor(const Ray& r)
{
	Vec3 unitDirection = UnitVector(r.Direction());
	auto a = 0.5 * (unitDirection.y() + 1.0);
	return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int main()
{
	double aspectRatio = 16.0 / 9.0;
	int imageWidth = 1600;

	// Calculate the image height, and ensure that it's at least 1.
	int imageHeight = int(imageWidth / aspectRatio);
	imageHeight = (imageHeight < 1) ? 1 : imageHeight;

	// Camera
	double focalLength = 1.0;
	double viewportHeight = 2.0;
	double viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);
	Point3 cameraCenter = Point3(0, 0, 0);

	Vec3 viewportU = Vec3(viewportWidth, 0, 0);
	Vec3 viewportV = Vec3(0, -viewportHeight, 0);

	Vec3 pixelDeltaU = viewportU / imageWidth;
	Vec3 pixelDeltaV = viewportV / imageHeight;

	auto viewportUpperLeft = cameraCenter - Vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
	auto pixel00Location = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

	std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

	for (int j = 0; j < imageHeight; j++)
	{
		std::clog << "\rScanlines remaining: " << (imageHeight - j) << " " << std::flush;

		for (int i = 0; i < imageWidth; i++)
		{
			auto pixelCenter = pixel00Location + (i * pixelDeltaU) + (j * pixelDeltaV);
			auto rayDirection = pixelCenter - cameraCenter;
			Ray r(cameraCenter, rayDirection);
			Color pixelColor = RayColor(r);

			WriteColor(std::cout, pixelColor);
		}
	}

	std::clog << "\rDone" << std::endl;

	return 0;
}
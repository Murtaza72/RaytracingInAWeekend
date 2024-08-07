#include "Camera.h"

Color Camera::RayColor(const Ray& r, int depth, HittableList& world)
{
	HitRecord record;

	// return if max depth is reached
	if (depth <= 0)
		return Color(0.0, 0.0, 0.0);

	if (world.Hit(r, Interval(0.001, infinity), record))
	{
		Vec3 direction = record.normal + RandomVecOnHemispher(record.normal);
		return 0.5 * RayColor(Ray(record.point, direction), depth - 1, world);
	}

	Vec3 unitDirection = UnitVector(r.Direction());
	auto a = 0.5 * (unitDirection.y() + 1.0);
	return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

void Camera::Initialize()
{
	// Calculate the image height, and ensure that it's at least 1.
	m_imageHeight = int(imageWidth / aspectRatio);
	m_imageHeight = (m_imageHeight < 1) ? 1 : m_imageHeight;

	m_pixelSampleScale = 1.0 / samplePerPixel;

	// Camera
	double focalLength = 1.0;
	double viewportHeight = 2.0;
	double viewportWidth = viewportHeight * (double(imageWidth) / m_imageHeight);
	m_cameraCenter = Point3(0, 0, 0);

	Vec3 viewportU = Vec3(viewportWidth, 0, 0);
	Vec3 viewportV = Vec3(0, -viewportHeight, 0);

	m_pixelDeltaU = viewportU / imageWidth;
	m_pixelDeltaV = viewportV / m_imageHeight;

	auto viewportUpperLeft = m_cameraCenter - Vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
	m_pixel00Location = viewportUpperLeft + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);
}

void Camera::Render(HittableList& world)
{
	Initialize();

	std::cout << "P3\n" << imageWidth << " " << m_imageHeight << "\n255\n";

	for (int j = 0; j < m_imageHeight; j++)
	{
		std::clog << "\rScanlines remaining: " << (m_imageHeight - j) << " " << std::flush;

		for (int i = 0; i < imageWidth; i++)
		{
			Color pixelColor(0.0, 0.0, 0.0);
			for (int sample = 0; sample < samplePerPixel; sample++)
			{
				Ray ray = GetRay(i, j);
				pixelColor += RayColor(ray, maxDepth, world);
			}

			WriteColor(std::cout, m_pixelSampleScale * pixelColor);
		}
	}

	std::clog << "\rDone" << std::endl;
}

Ray Camera::GetRay(int i, int j)
{
	Vec3 offset = RandomSample();
	auto pixelSample = m_pixel00Location
		+ ((i + offset.x()) * m_pixelDeltaU)
		+ ((j + offset.y()) * m_pixelDeltaV);

	auto rayDirection = pixelSample - m_cameraCenter;

	return Ray(m_cameraCenter, rayDirection);
}

Vec3 Camera::RandomSample()
{
	return Vec3(Random() - 0.5, Random() + 0.5, 0.0);
}
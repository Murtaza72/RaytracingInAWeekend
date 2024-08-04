#include "Camera.h"

Color Camera::RayColor(const Ray& r, HittableList& world)
{
	HitRecord record;
	if (world.Hit(r, Interval(0, infinity), record))
	{
		return 0.5 * (record.normal + Color(1, 1, 1));
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
			auto pixelCenter = m_pixel00Location + (i * m_pixelDeltaU) + (j * m_pixelDeltaV);
			auto rayDirection = pixelCenter - m_cameraCenter;
			Ray r(m_cameraCenter, rayDirection);
			Color pixelColor = RayColor(r, world);

			WriteColor(std::cout, pixelColor);
		}
	}

	std::clog << "\rDone" << std::endl;
}

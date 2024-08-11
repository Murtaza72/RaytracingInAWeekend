#include "Camera.h"
#include "Material.h"

Color Camera::RayColor(const Ray& ray, int depth, HittableList& world)
{
	HitRecord record;

	// return if max depth is reached
	if (depth <= 0)
		return Color(0.0, 0.0, 0.0);

	if (world.Hit(ray, Interval(0.000001, infinity), record))
	{
		Ray scattered;
		Color attenuation;
		if (record.material->Scattter(ray, record, attenuation, scattered))
			return attenuation * RayColor(scattered, depth - 1, world);

		return Color(0.0, 0.0, 0.0);
	}

	Vec3 unitDirection = UnitVector(ray.Direction());
	auto a = 0.5 * (unitDirection.y() + 1.0);
	return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

void Camera::Initialize()
{
	// Calculate the image height, and ensure that it's at least 1.
	m_ImageHeight = int(imageWidth / aspectRatio);
	m_ImageHeight = (m_ImageHeight < 1) ? 1 : m_ImageHeight;

	m_PixelSampleScale = 1.0 / samplePerPixel;

	// Viewport dimensions
	double focalLength = (lookFrom - lookAt).Length();
	double theta = DegreesToRadians(vertFieldOfView);
	double h = tan(theta / 2);
	double viewportHeight = 2 * h * focalLength;
	double viewportWidth = viewportHeight * (double(imageWidth) / m_ImageHeight);

	m_CameraCenter = lookFrom;

	// unit basis vector
	w = UnitVector(lookFrom - lookAt);
	u = UnitVector(cross(viewUp, w));
	v = cross(w, u);

	Vec3 viewportU = viewportWidth * u;
	Vec3 viewportV = viewportHeight * -v;

	m_PixelDeltaU = viewportU / imageWidth;
	m_PixelDeltaV = viewportV / m_ImageHeight;

	Vec3 viewportUpperLeft = m_CameraCenter - (focalLength * w) - viewportU / 2 - viewportV / 2;
	m_Pixel00Location = viewportUpperLeft + 0.5 * (m_PixelDeltaU + m_PixelDeltaV);
}

void Camera::Render(HittableList& world)
{
	Initialize();

	std::cout << "P3\n" << imageWidth << " " << m_ImageHeight << "\n255\n";

	for (int j = 0; j < m_ImageHeight; j++)
	{
		std::clog << "\rScanlines remaining: " << (m_ImageHeight - j) << " " << std::flush;

		for (int i = 0; i < imageWidth; i++)
		{
			Color pixelColor(0.0, 0.0, 0.0);
			for (int sample = 0; sample < samplePerPixel; sample++)
			{
				Ray ray = GetRay(i, j);
				pixelColor += RayColor(ray, maxDepth, world);
			}

			WriteColor(std::cout, m_PixelSampleScale * pixelColor);
		}
	}

	std::clog << "\rDone" << std::endl;
}

Ray Camera::GetRay(int i, int j)
{
	Vec3 offset = RandomSample();
	auto pixelSample = m_Pixel00Location
		+ ((i + offset.x()) * m_PixelDeltaU)
		+ ((j + offset.y()) * m_PixelDeltaV);

	auto rayDirection = pixelSample - m_CameraCenter;

	return Ray(m_CameraCenter, rayDirection);
}

Vec3 Camera::RandomSample()
{
	return Vec3(Random() - 0.5, Random() + 0.5, 0.0);
}
#include "Camera.h"
#include "Material.h"

#include <execution>

Color Camera::RayColor(const Ray& ray, int depth, HittableList& world) const
{
	HitRecord record;

	// return if max depth is reached
	if (depth <= 0)
		return Color(0.0, 0.0, 0.0);

	// Interval min !=0 to avoid shadow acne
	if (world.Hit(ray, Interval(0.000001, infinity), record))
	{
		Ray scattered;
		Color attenuation;
		if (record.material->Scattter(ray, record, attenuation, scattered))
			return attenuation * RayColor(scattered, depth - 1, world);

		return Color(0.0, 0.0, 0.0);
	}

	// Background gradient
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
	double theta = DegreesToRadians(vertFieldOfView);
	double h = tan(theta / 2);
	double viewportHeight = 2 * h * focusDistance;
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

	Vec3 viewportUpperLeft = m_CameraCenter - (focusDistance * w) - viewportU / 2 - viewportV / 2;
	m_Pixel00Location = viewportUpperLeft + 0.5 * (m_PixelDeltaU + m_PixelDeltaV);

	double defocusRadius = focusDistance * tan(DegreesToRadians(defocusAngle / 2));
	m_DefocusHorizontal = u * defocusRadius;
	m_DefocusVertical = v * defocusRadius;

	m_ImageData = new Color[imageWidth * m_ImageHeight];
}

void Camera::RenderMT(HittableList& world)
{
	Initialize();

	for (int i = 0; i < imageWidth; i++) m_ImageHorizontalIter.push_back(i);
	for (int i = 0; i < m_ImageHeight; i++) m_ImageVerticalIter.push_back(i);

	std::cout << "P3\n" << imageWidth << " " << m_ImageHeight << "\n255\n";

	std::for_each(std::execution::par, m_ImageVerticalIter.begin(), m_ImageVerticalIter.end(),
		[this, &world](int y) {

			std::clog << "\rScanlines remaining: " << (m_ImageHeight - y) << " " << std::flush;

			std::for_each(std::execution::par, m_ImageHorizontalIter.begin(), m_ImageHorizontalIter.end(),
				[this, y, &world](int x)
				{
					Color pixelColor(0.0, 0.0, 0.0);
					for (int sample = 0; sample < samplePerPixel; sample++)
					{
						Ray ray = GetRay(x, y);
						pixelColor += RayColor(ray, maxDepth, world);
					}

					m_ImageData[x + y * imageWidth] = m_PixelSampleScale * pixelColor;

				});
		});

	WriteColor(std::cout, m_ImageData, imageWidth, m_ImageHeight);

	std::clog << "\rDone" << std::endl;
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

Ray Camera::GetRay(int i, int j) const
{
	// Anti-aliasing
	Vec3 offset = RandomSample();
	auto pixelSample = m_Pixel00Location
		+ ((i + offset.x()) * m_PixelDeltaU)
		+ ((j + offset.y()) * m_PixelDeltaV);

	Point3 rayOrigin = (defocusAngle <= 0) ? m_CameraCenter : RandomDefocusDiskSample();
	auto rayDirection = pixelSample - rayOrigin;

	return Ray(rayOrigin, rayDirection);
}

Vec3 Camera::RandomSample() const
{
	return Vec3(Random() - 0.5, Random() + 0.5, 0.0);
}

Vec3 Camera::RandomDefocusDiskSample() const
{
	Vec3 vec = RandomVectorInUnitDisk();

	return m_CameraCenter + (vec.x() * m_DefocusHorizontal) + (vec.y() * m_DefocusVertical);
}
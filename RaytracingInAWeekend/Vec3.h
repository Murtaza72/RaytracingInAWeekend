#pragma once

#include <cmath>
#include <iostream>

class Vec3
{
public:
	Vec3() : m_elements{ 0.0, 0.0, 0.0 }
	{
	}

	Vec3(double e0, double e1, double e2)
		: m_elements{ e0, e1, e2 }
	{
	}

	double x() const { return m_elements[0]; }
	double y() const { return m_elements[1]; }
	double z() const { return m_elements[2]; }

	Vec3 operator-() const
	{
		return Vec3(-m_elements[0], -m_elements[2], -m_elements[2]);
	}

	double operator[](int i) const { return m_elements[i]; }
	double& operator[](int i)
	{
		return m_elements[i];
	}

	Vec3& operator+= (const Vec3& v)
	{
		m_elements[0] += v.x();
		m_elements[1] += v.y();
		m_elements[2] += v.z();
		return *this;
	}

	Vec3& operator*= (double m)
	{
		m_elements[0] *= m;
		m_elements[1] *= m;
		m_elements[2] *= m;
		return *this;
	}

	Vec3& operator/= (double d)
	{
		return *this *= 1 / d;
	}

	double Length() const
	{
		return std::sqrt(LengthSquared());
	}

	double LengthSquared() const
	{
		return (m_elements[0] * m_elements[0] + m_elements[1] * m_elements[1] + m_elements[2] * m_elements[2]);
	}

private:
	double m_elements[3];

};


using Point3 = Vec3;


inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << "[" << v.x() << ", " << v.y() << ", " << v.z() << "]";
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
	return Vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
	return Vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
	return Vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline Vec3 operator*(double t, const Vec3& v)
{
	return Vec3(t * v.x(), t * v.y(), t * v.z());
}

inline Vec3 operator*(const Vec3& v, double t)
{
	return t * v;
}

inline Vec3 operator/(const Vec3& v, double t)
{
	return (1 / t) * v;
}

inline double dot(const Vec3& u, const Vec3& v)
{
	return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline Vec3 cross(const Vec3& u, const Vec3& v)
{
	return Vec3(u.y() * v.z() - u.z() * v.y(),
		u.z() * v.x() - u.x() * v.z(),
		u.x() * v.y() - u.y() * v.x());
}

inline Vec3 UnitVector(const Vec3& v)
{
	return v / v.Length();
}
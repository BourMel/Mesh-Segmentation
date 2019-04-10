#ifndef VEC3_H
#define VEC3_H

#include <cmath>

template <typename T>
class Vec3
{
  public:
  
	Vec3();

	Vec3(T x, T y, T z);

	// getter
	T x() const;
	T y() const;
	T z() const;

	// setter
	void x(T x);
	void y(T y);
	void z(T z);

	// operator overload

	Vec3 operator+(const Vec3 &rhs);
	Vec3 operator-(const Vec3 &rhs);

	// dot product
	float operator*(const Vec3 &rhs);
	// cross product
	Vec3 operator^(const Vec3 &rhs);

	// scalar multiplication
	Vec3 operator*(float k);
	// scalar division
	Vec3 operator/(float k);

	// exact length of a vector
	float length();

	// squared length, to use for length comparison
	float sqLength();
 
	// angle between vector *this and b
	float angle(Vec3 b);

  private:
	T m_x, m_y, m_z;
};

using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;

#endif
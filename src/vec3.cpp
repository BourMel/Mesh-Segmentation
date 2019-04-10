#include "vec3.hpp"

#include <stdexcept>

template <typename T>
Vec3<T>::Vec3()
{
    m_x = dynamic_cast<T>(0);
    m_y = dynamic_cast<T>(0);
    m_z = dynamic_cast<T>(0);
}

template <typename T>
Vec3<T>::Vec3(T x, T y, T z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

template <typename T>
T Vec3<T>::x() const { return m_x; }
template <typename T>
T Vec3<T>::y() const { return m_y; }
template <typename T>
T Vec3<T>::z() const { return m_z; }

template <typename T>
void Vec3<T>::x(T x) { m_x = x; }
template <typename T>
void Vec3<T>::y(T y) { m_y = y; }
template <typename T>
void Vec3<T>::z(T z) { m_z = z; }

template <typename T>
Vec3<T> Vec3<T>::operator+(const Vec3 &rhs)
{
    return Vec3(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z);
}

template <typename T>
Vec3<T> Vec3<T>::operator-(const Vec3 &rhs)
{
    return Vec3(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z);
}

template <typename T>
float Vec3<T>::operator*(const Vec3 &rhs)
{
    return m_x * rhs.m_x + m_y * rhs.m_y + m_z * rhs.m_z;
}

template <typename T>
Vec3<T> Vec3<T>::operator^(const Vec3 &rhs)
{
    return Vec3(
        m_y * rhs.m_z - m_z * rhs.m_y,
        m_z * rhs.m_x - m_x * rhs.m_z,
        m_x * rhs.m_y - m_y * rhs.m_x);
}

template <typename T>
Vec3<T> Vec3<T>::operator*(float k)
{
    return Vec3(m_x * k, m_y * k, m_z * k);
}

template <typename T>
Vec3<T> Vec3<T>::operator/(float k)
{
    return Vec3(m_x / k, m_y / k, m_z / k);
}

template <typename T>
float Vec3<T>::length()
{
    return sqrt(sqLength());
}

template <typename T>
float Vec3<T>::sqLength()
{
    return m_x * m_x + m_y * m_y + m_z * m_z;
}

template <typename T>
float Vec3<T>::angle(Vec3<T> b)
{
    return acos((*this * b) / (length(*this) * length(b)));
}
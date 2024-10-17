#include "Math.h"

Vec2::Vec2()
{
    x = 0;
    y = 0;
}

Vec2::Vec2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vec2::~Vec2()
{
    x = 0;
    y = 0;
}

Vec2 Vec2::operator+(float rhs)
{
    return Vec2(x + rhs, y + rhs);
}

Vec2 Vec2::operator*(float rhs)
{
    return Vec2(x * rhs, y * rhs);
}

Vec3::Vec3()
{
    x = 0;
    y = 0;
    z = 0;
}

Vec3::Vec3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3::~Vec3()
{
    x = 0;
    y = 0;
    z = 0;
}

Vec3 Vec3::operator+(float rhs)
{
    return Vec3(x + rhs, y + rhs, z + rhs);
}

Vec3 Vec3::operator*(float rhs)
{
    return Vec3(x * rhs, y * rhs, z * rhs);
}
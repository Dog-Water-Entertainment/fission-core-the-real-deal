#include "Math.h"
#include <cmath>

// Michael Guillory

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

Vec2 Vec2::operator-(Vec2 rhs)
{
    return Vec2(x - rhs.x, y - rhs.y);
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

Vec3 Vec3::operator-(Vec3 rhs)
{
    return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
}

float distance(Vec2 a, Vec2 b)
{
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

float angleFromAtoB(Vec2 a, Vec2 b)
{
    return atan2(b.y - a.y, b.x - a.x);
}
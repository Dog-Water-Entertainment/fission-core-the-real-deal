#ifndef MATH_H
#define MATH_H

const float PI = 3.14159265359; // as much accuracy as I want

// Michael Guillory

class Vec2
{
public:
    float x;
    float y;

    Vec2();
    Vec2(float x, float y);
    ~Vec2();

    Vec2 operator+(float rhs);
    Vec2 operator*(float rhs);
    Vec2 operator-(Vec2 rhs);
};

class Vec3
{
public:
    float x;
    float y;
    float z;

    Vec3();
    Vec3(float x, float y, float z);
    ~Vec3();

    Vec3 operator+(float rhs);
    Vec3 operator*(float rhs);
    Vec3 operator-(Vec3 rhs);
};

float distance(Vec2 a, Vec2 b);

float angleFromAtoB(Vec2 a, Vec2 b);

#endif // MATH_H
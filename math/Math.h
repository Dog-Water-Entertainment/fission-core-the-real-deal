#ifndef MATH_H
#define MATH_H

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

};

#endif // MATH_H
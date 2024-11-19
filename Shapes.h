#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "Math.h"
#include <GL/glx.h>


// Origin: Michael Guillory
class Shape
{
    protected:
        Vec2 position;
    public:
        Shape();
        ~Shape();

        virtual void draw() = 0;
};

class CollisionBox {
    public:
        Vec2 vmin;
        Vec2 vmax;
        CollisionBox() {
            vmin = Vec2(0, 0);
            vmax = Vec2(0, 0);
        }
        CollisionBox(Vec2 min, Vec2 max) {
            vmin = min;
            vmax = max;
        }
};

class Rectangle : public Shape
{
    private:
        GLuint * texture;
        Vec2 size;
        Vec2 vertices[4];
        Vec2 globalPosition;
        Vec3 color;
        void updateVertices();
    public:
        CollisionBox collisionBox;
        float rotation;
        Rectangle(Vec2 position, int width, int height);
        Rectangle();
        ~Rectangle();

        void setTexture(GLuint * texture);

        // Will move with the vector
        // provided
        void move(Vec2 position);

        Vec2 getPosition();

        // sets the color of the rectangle
        // default is white
        void setColor(Vec3 color);

        void draw();
        void drawRotated(float angle);
};

class Line : public Shape
{
    private:
        Vec3 color;
        Vec2 start;
        Vec2 end;
        int width;
    public:
        Line(Vec2 start, Vec2 end, int width);
        Line();
        ~Line();


        void setColor(Vec3 color);
        void draw();
};


#endif // _SHAPE_H_
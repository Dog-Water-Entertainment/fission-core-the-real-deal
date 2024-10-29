#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "./Math.h"
#include <GL/glx.h>

class Shape
{
    protected:
        Vec2 position;
    public:
        Shape();
        ~Shape();

        virtual void draw() = 0;
};

class Rectangle : public Shape
{
    private:
        GLuint * texture;
        Vec2 size;
        Vec2 vertices[4];
        Vec2 globalPosition;
        void updateVertices();
    public:
        Rectangle(Vec2 position, int width, int height);
        Rectangle();
        ~Rectangle();

        // Will move with the vector
        // provided
        void move(Vec2 position);

        Vec2 getPosition();

        void rotateAroundLocal(float angle);
        void rotateAroundGlobal(float angle);
        void draw();
        void drawRotated(float angle);
};


#endif // _SHAPE_H_
#include "./Shapes.h"
#include <cmath>

Shape::Shape()
{
    position = Vec2(0, 0);
}

Shape::~Shape()
{
}

void Shape::draw()
{
}

Rectangle::Rectangle(Vec2 position, int width, int height)
{
    texture = nullptr;
    this->position = position;
    size = Vec2(width, height);
    updateVertices();

    globalPosition = Vec2(0, 0);
}

Rectangle::Rectangle()
{
    texture = nullptr;
    position = Vec2(0, 0);
    size = Vec2(0, 0);
    updateVertices();

    globalPosition = Vec2(0, 0);
}


Rectangle::~Rectangle()
{
}

void Rectangle::updateVertices()
{
    float _width = size.x / 2;
    float _height = size.y / 2;

    vertices[0] = Vec2(position.x -_width, position.y - _height);
    vertices[1] = Vec2(position.x + _width, position.y - _height);
    vertices[2] = Vec2(position.x + _width, position.y + _height);
    vertices[3] = Vec2(position.x - _width, position.y + _height);

    globalPosition = Vec2(800 - position.x, 600 - position.y);
}

void Rectangle::move(Vec2 position)
{
    Vec2 currPos = this->position;
    this->position = Vec2(currPos.x + position.x, currPos.y + position.y);
    updateVertices();
}

Vec2 Rectangle::getPosition()
{
    return position;
}

void Rectangle::rotateAroundLocal(float angle)
{
    float _cos = cos(angle);
    float _sin = sin(angle);

    for (int i = 0; i < 4; i++) {
        float x = vertices[i].x - position.x;
        float y = vertices[i].y - position.y;

        vertices[i].x = x * _cos - y * _sin + position.x;
        vertices[i].y = x * _sin + y * _cos + position.y;
    }
}

void Rectangle::rotateAroundGlobal(float angle)
{
    float _cos = cos(angle);
    float _sin = sin(angle);

    for (int i = 0; i < 4; i++) {
        float x = vertices[i].x - globalPosition.x;
        float y = vertices[i].y - globalPosition.y;

        vertices[i].x = x * _cos - y * _sin + globalPosition.x;
        vertices[i].y = x * _sin + y * _cos + globalPosition.y;
    }
}

void Rectangle::draw()
{
    if(texture != nullptr) {
        return; // Not implemented yet
        glBindTexture(GL_TEXTURE_2D, *texture);
        glPushMatrix();
        glColor4f(1.0f, 0.2f, 0.2f, 0.8f);
        glBegin(GL_QUADS);
            for (int i = 0; i < 4; i++)
            {
                glVertex2f(vertices[i].x, vertices[i].y);
            }
        glEnd();
        glPopMatrix();
    } else {
        glPushMatrix();
        glColor4f(1.0f, 0.2f, 0.2f, 0.8f);
        glBegin(GL_QUADS);
            for (int i = 0; i < 4; i++)
            {
                glVertex2f(vertices[i].x, vertices[i].y);
            }
        glEnd();
        glPopMatrix();
    }
}

void Rectangle::drawRotated(float angle)
{
    Vec2 drawingVerts[4];

    float _cos = cos(angle);
    float _sin = sin(angle);

    for (int i = 0; i < 4; i++) {
        float x = vertices[i].x - position.x;
        float y = vertices[i].y - position.y;

        drawingVerts[i].x = x * _cos - y * _sin + position.x;
        drawingVerts[i].y = x * _sin + y * _cos + position.y;
    }

    glPushMatrix();
    glColor4f(1.0f, 0.2f, 0.2f, 0.8f);
    glBegin(GL_QUADS);
        for (int i = 0; i < 4; i++)
        {
            glVertex2f(drawingVerts[i].x, drawingVerts[i].y);
        }
    glEnd();
    glPopMatrix();
}

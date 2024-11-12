#include "Shapes.h"
#include <GL/glx.h>
#include <iostream>
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

    /*

    3, 2
    0, 1

    */

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

void Rectangle::setTexture(GLuint * texture)
{
    this->texture = texture;
}

void Rectangle::draw()
{
    Vec2 drawingVerts[4];

    float _cos = cos(rotation);
    float _sin = sin(rotation);

    for (int i = 0; i < 4; i++) {
        float x = vertices[i].x - position.x;
        float y = vertices[i].y - position.y;

        drawingVerts[i].x = x * _cos - y * _sin + position.x;
        drawingVerts[i].y = x * _sin + y * _cos + position.y;
    }

    /*

    0, 1
    3, 2

    */

    glPushMatrix();
    if (texture != nullptr) {
        glEnable(GL_TEXTURE_2D);
        glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glBindTexture(GL_TEXTURE_2D, *texture);
        glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
    } else {
        glColor4f(1.0f, 0.2f, 0.2f, 0.8f);
    }

    glBegin(GL_QUADS);
        if (texture != nullptr) {

            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(drawingVerts[0].x, drawingVerts[0].y);

            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(drawingVerts[1].x, drawingVerts[1].y);

            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(drawingVerts[2].x, drawingVerts[2].y);

            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(drawingVerts[3].x, drawingVerts[3].y);
        } else {
            for (int i = 0; i < 4; i++)
            {
                glVertex2f(drawingVerts[i].x, drawingVerts[i].y);
            }
        }
    glEnd();

    if (texture != nullptr) {
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_ALPHA_TEST);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glPopMatrix();
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

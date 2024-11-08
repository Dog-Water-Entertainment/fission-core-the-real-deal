#include "Inputs.h"
#include "Math.h"

static unsigned char keys[65536] = {0};

static Vec2 mousePos;

static bool mousePressed = false;

bool get_key(unsigned int key)
{
    return keys[key];
}

void set_key(unsigned int key, bool value)
{
    keys[key] = value;
}

void set_mouse_pos(int x, int y)
{
    mousePos = Vec2(x, y);
}

Vec2 get_mouse_pos()
{
    return mousePos;
}

bool is_mouse_pressed()
{
    return mousePressed;
}

void set_mouse_pressed(bool value)
{
    mousePressed = value;
}

static bool firstPress = true;
bool get_mouse_inpulse()
{
    if (mousePressed && firstPress) {
        firstPress = false;
        return true;
    }
    return false;
}

void reset_first_mouse_press()
{
    firstPress = true;
}
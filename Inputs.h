#ifndef _INPUTS_H_
#define _INPUTS_H_

#include "Math.h"

bool get_key(unsigned int key);

void set_key(unsigned int key, bool value);

void set_mouse_pos(int x, int y);

Vec2 get_mouse_pos();

void set_mouse_pressed(bool value);

bool is_mouse_pressed();

bool get_mouse_inpulse();

void reset_first_mouse_press();

#endif // _INPUTS_H_
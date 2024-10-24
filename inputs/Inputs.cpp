#include "Inputs.h"
#include <iostream>

static unsigned char keys[65536] = {0};


bool get_key(unsigned int key)
{
    return keys[key];
}

void set_key(unsigned int key, bool value)
{
    keys[key] = value;
}
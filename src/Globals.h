#ifndef GLOBALS_H
#define GLOBALS_H
#include <X11/Xlib.h>
#include "MainWindow.h"
#include "Input.h"

struct Global
{
    Global();
    MainWindow *main_window;
    Input *input;
};

extern Global g;

#endif
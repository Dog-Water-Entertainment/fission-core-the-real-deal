#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>

class MainWindow
{
public:
    Display *display;
    int screen;
    Window root;
    Window window;

    MainWindow();
    ~MainWindow();
};

#endif
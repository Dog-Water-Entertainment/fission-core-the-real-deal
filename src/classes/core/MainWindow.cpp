#include <X11/Xlib.h>
#include <iostream>
#include "MainWindow.h"

MainWindow::MainWindow()
{
    this->display = XOpenDisplay(NULL);

    if (this->display == NULL)
    {
        std::cout << "Could not open X11 display.\n";
        exit(EXIT_FAILURE);
    }

    this->screen = DefaultScreen(this->display);
    this->root = RootWindow(this->display, this->screen);

    XSetWindowAttributes attributes;
    attributes.background_pixel = BlackPixel(this->display, this->screen);

    this->window = XCreateWindow(this->display, this->root, 0, 0, 500, 500, 15, CopyFromParent, InputOutput, CopyFromParent, CWBackPixel, &attributes);
    XMapWindow(this->display, this->window);
}

MainWindow::~MainWindow()
{
    std::cout << "Termination\n";
    XUnmapWindow(this->display, this->window);
    XDestroyWindow(this->display, this->window);

    XCloseDisplay(this->display);
}
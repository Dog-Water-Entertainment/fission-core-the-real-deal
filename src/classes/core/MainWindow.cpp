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

    this->window = XCreateWindow(this->display, this->root, 0, 0, 250, 250, 15, CopyFromParent, InputOutput, CopyFromParent, CWBackPixel, &attributes);
    XMapWindow(this->display, this->window);

    // Full screen code citation: https://stackoverflow.com/questions/9065669/x11-glx-fullscreen-mode
    Atom wm_state = XInternAtom(this->display, "_NET_WM_STATE", true);
    Atom wm_full_screen = XInternAtom(this->display, "_NET_WM_STATE_FULLSCREEN", true);
    XChangeProperty(this->display, this->window, wm_state, 4, 32, PropModeReplace, (unsigned char *)&wm_full_screen, 1);
}

MainWindow::~MainWindow()
{
    XUnmapWindow(this->display, this->window);
    XDestroyWindow(this->display, this->window);
    XCloseDisplay(this->display);
}
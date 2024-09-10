#include "Input.h"
#include <thread>
#include <assert.h>
#include <X11/Xlib.h>
#include "Globals.h"

Input::Input()
    : m_started(false)
{
}

Input::~Input()
{
}

void Input::start()
{
    if (m_started)
        throw "The input manager has already started.";

    XEvent event;

    while (XNextEvent(g.main_window.display, &event) == 0)
    {
    }
}
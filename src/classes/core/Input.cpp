#include "Input.h"
#include <thread>
#include <assert.h>
#include <X11/Xlib.h>
#include <iostream>
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

    while (XPending(g.main_window->display))
    {
        // do stuff
    }
}

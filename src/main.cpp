/**
 * Author: Elias Farmer
 * Date: 9/9/24
 * Handles initializing all of the classes.
 */
#include <X11/Xlib.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "MainWindow.h"
#include "Input.h"
#include "Globals.h"
#include <unistd.h>

int main()
{
    g.input->start();

    // TODO: Remove me.

    while (true)
    {
    }

    return EXIT_SUCCESS;
}
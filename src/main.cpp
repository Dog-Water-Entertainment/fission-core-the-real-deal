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

Global globals;

int main()
{
    g.main_window = MainWindow();
    g.input = Input();
    g.input.start();

    return EXIT_SUCCESS;
}
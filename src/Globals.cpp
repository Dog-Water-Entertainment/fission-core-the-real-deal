#include "Globals.h"
#include "Input.h"
#include "MainWindow.h"

Global g;

Global::Global()
{
    main_window = new MainWindow();
    input = new Input();
};
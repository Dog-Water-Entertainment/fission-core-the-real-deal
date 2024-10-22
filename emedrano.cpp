// Name: Enrique Medrano
// Main source file
#include "emedrano.h"
#include "fonts.h"
#include <GL/glx.h>
#include <iostream>

StartScreen::StartScreen() {
    gameStarted = false;
}

void StartScreen::renderStartScreen(int xres, int yres) {
    // Render the start screen background
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    Rect r;
    r.bot = yres - 200;
    r.left = xres / 2 - 100;
    r.center = 0;
    unsigned int c = 0x00ffff44;

    // Display start screen text
    ggprint8b(&r, 16, c, "Welcome to the Game!");
    r.bot -= 50;
    ggprint8b(&r, 16, c, "Press Enter to Start");
}

bool StartScreen::handleStartInput(int key) {
    // Check for Enter key to start the game
    if (key == XK_Return) {
        gameStarted = true;
        return true; 
    }
    return false; 
}


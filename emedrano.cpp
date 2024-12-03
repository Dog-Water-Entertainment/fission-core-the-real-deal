// Name: Enrique Medrano
// Main Source File

#include "emedrano.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <X11/keysym.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>

// Utility: Get current time in seconds
double getCurrentTime() {
    return static_cast<double>(clock()) / CLOCKS_PER_SEC;
}

// Debugging utility: Enables or disables verbose debugging messages
void enableVerboseDebugging(bool verbose) {
    if (verbose) {
        std::cout << "[DEBUG] Verbose Debugging Enabled" << std::endl;
    } else {
        std::cout << "[DEBUG] Verbose Debugging Disabled" << std::endl;
    }
}

// Utility: Render text on screen using Rect positioning
void renderText(Rect *r, unsigned int color, const char *text) {
    ggprint8b(r, 16, color, text);
}

// Utility: Render a rectangle on screen with a specific color
void renderRectangle(float x, float y, float width, float height, unsigned int color) {
    glColor4ub((color >> 24) & 0xFF, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// Utility: Render a gradient background
void renderBackgroundGradient(float r1, float g1, float b1, float r2, float g2, float b2) {
    glBegin(GL_QUADS);
    glColor3f(r1, g1, b1); // Top color
    glVertex2f(-1.0, 1.0);
    glVertex2f(1.0, 1.0);
    glColor3f(r2, g2, b2); // Bottom color
    glVertex2f(1.0, -1.0);
    glVertex2f(-1.0, -1.0);
    glEnd();
}

// HighScoreManager: Manages game high scores, including loading, saving, and displaying
HighScoreManager::HighScoreManager() {
    loadScores(); // Load scores from a file during initialization
}

// Loads high scores from a file
void HighScoreManager::loadScores() {
    std::ifstream file(fileName);
    if (file.is_open()) {
        scores.clear();
        std::string name;
        int score;
        while (file >> name >> score) {
            scores.push_back({name, score});
        }
        file.close();
    }
}

// Saves high scores to a file
void HighScoreManager::saveScores() {
    std::ofstream file(fileName);
    if (file.is_open()) {
        for (const auto &entry : scores) {
            file << entry.first << " " << entry.second << "\n";
        }
        file.close();
    }
}

// Adds a new score to the high scores list and ensures the list is sorted and limited to 10
void HighScoreManager::addScore(const std::string &name, int score) {
    scores.push_back({name, score});
    std::sort(scores.begin(), scores.end(), [](auto &a, auto &b) {
            return b.second < a.second;
            });
    if (scores.size() > 10) {
        scores.resize(10);
    }
    saveScores(); // Save the updated scores
}

// Renders the high scores on the screen
void HighScoreManager::renderHighScores(int xres, int yres) {
    glClearColor(0.1, 0.1, 0.1, 1.0); // Set background color
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the high scores title
    Rect titleRect;
    titleRect.bot = yres - 100;
    titleRect.left = xres / 2 - 100;
    titleRect.center = 0;
    renderText(&titleRect, 0x00ff77ff, "High Scores");

    // Render each high score entry
    for (size_t i = 0; i < scores.size(); ++i) {
        Rect scoreRect;
        scoreRect.bot = yres / 2 - (i * 30);
        scoreRect.left = xres / 2 - 100;
        scoreRect.center = 0;

        std::string scoreLine = scores[i].first + ": " + std::to_string(scores[i].second);
        renderText(&scoreRect, 0x00ffffff, scoreLine.c_str());
    }

    // Render footer instructions
    Rect footerRect;
    footerRect.bot = 50;
    footerRect.left = xres / 2 - 120;
    footerRect.center = 0;
    renderText(&footerRect, 0x00ffff44, "Press Esc to Return.");
}

// StartScreen: Manages the game's start screen with navigable menu options
StartScreen::StartScreen()
    : gameStarted(false), selectedOption(0),
    animationStartTime(getCurrentTime()),
    menuOptions{"Start Game", "High Scores", "Exit"},
    menuCount(3) {}

    // Renders the start screen with menu options
    void StartScreen::renderStartScreen(int xres, int yres) {
        renderBackgroundGradient(0.1, 0.1, 0.1, 0.0, 0.0, 0.0); // Render gradient background

        // Render the title
        Rect titleRect;
        titleRect.bot = yres - 100;
        titleRect.left = xres / 2 - 100;
        titleRect.center = 0;
        renderText(&titleRect, 0x00ff77ff, "Welcome to Fission Core!");

        // Render each menu option
        for (int i = 0; i < menuCount; ++i) {
            Rect optionRect;
            optionRect.bot = yres / 2 - (i * 30);
            optionRect.left = xres / 2 - 50;
            optionRect.center = 0;

            unsigned int optionColor = (i == selectedOption) ? 0x00ffffff : 0x00777777; // Highlight selected option
            renderText(&optionRect, optionColor, menuOptions[i]);
        }

        // Render footer instructions
        Rect footerRect;
        footerRect.bot = 50;
        footerRect.left = xres / 2 - 120;
        footerRect.center = 0;
        renderText(&footerRect, 0x00ffff44, "Use Arrow Keys to Navigate. Press Enter to Select.");
    }

// Handles input for navigating and selecting start screen options
bool StartScreen::handleStartInput(int key) {
    switch (key) {
        case XK_Return: // Confirm selected option
            if (selectedOption == 0) {
                gameStarted = true; // Start the game
                return true;
            } else if (selectedOption == 1) {
                highScoreManager.renderHighScores(800, 600); // Show high scores
            } else if (selectedOption == 2) {
                exit(0); // Exit the game
            }
            break;
        case XK_Up: // Navigate up
            selectedOption = (selectedOption - 1 + menuCount) % menuCount;
            break;
        case XK_Down: // Navigate down
            selectedOption = (selectedOption + 1) % menuCount;
            break;
        default:
            break;
    }
    return false;
}

// Easter Egg: Renders a hidden screen with fun messages
void showEasterEgg(int xres, int yres) {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set black background
    glClear(GL_COLOR_BUFFER_BIT);

    // Render Easter egg title
    Rect titleRect;
    titleRect.bot = yres - 100;
    titleRect.left = xres / 2 - 100;
    titleRect.center = 0;
    renderText(&titleRect, 0x00ff77ff, "Easter Egg Found!");

    // Render messages
    const char *messages[] = {
        "You discovered a hidden feature!",
        "Enjoy this moment of glory.",
        "Remember to have fun while coding."
    };

    for (int i = 0; i < 3; ++i) {
        Rect textRect;
        textRect.bot = yres / 2 - (i * 30);
        textRect.left = xres / 2 - 150;
        textRect.center = 0;
        renderText(&textRect, 0x00ffffff, messages[i]);
    }
}


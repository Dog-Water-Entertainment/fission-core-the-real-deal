#ifndef EMEDRANO_H
#define EMEDRANO_H

#include "fonts.h"
#include <vector>
#include <string>

// Utility: Function declarations
double getCurrentTime();
void renderText(Rect *r, unsigned int color, const char *text);
void renderRectangle(float x, float y, float width, float height, unsigned int color);
void renderBackgroundGradient(float r1, float g1, float b1, float r2, float g2, float b2);
void enableVerboseDebugging(bool verbose);

// Class: HighScoreManager
class HighScoreManager {
private:
    std::vector<std::pair<std::string, int>> scores;
    const std::string fileName = "highscores.txt";

public:
    HighScoreManager();
    void loadScores();
    void saveScores();
    void addScore(const std::string &name, int score);
    void renderHighScores(int xres, int yres);
};

// Class: StartScreen
class StartScreen {
private:
    bool gameStarted;
    int selectedOption;
    double animationStartTime;
    const char *menuOptions[3];
    const int menuCount;
    HighScoreManager highScoreManager; // Add highScoreManager as a private member

public:
    StartScreen();
    void renderStartScreen(int xres, int yres);
    bool handleStartInput(int key);
};

// Function declarations for additional features
void showEasterEgg(int xres, int yres);
void simulateNewFeature();

#endif // EMEDRANO_H


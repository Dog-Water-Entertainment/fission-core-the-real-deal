// Enrique Medrano
// Header file

#ifndef EMEDRANO_H
#define EMEDRANO_H

class StartScreen {
    public:
        bool gameStarted;
        StartScreen();
        void renderStartScreen(int xres, int yres);
        bool handleStartInput(int key);
};

#endif // EMEDRANO_H


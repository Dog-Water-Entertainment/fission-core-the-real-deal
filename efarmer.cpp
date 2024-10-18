#include <iostream>
#include <set>
#include <vector>
#include <functional>
#include "fonts.h"
#include <GL/glx.h>
#include <string>
#include "efarmer.h"

PauseMenu* PauseMenu::m_instance = nullptr;
const int PauseMenu::SELECTED_BUTTON_COLOR = 0x0000FF00;
const int PauseMenu::BUTTON_COLOR = 0x00FFFF00;

PauseMenu* PauseMenu::get()
{
        if (m_instance == nullptr)
        {
                m_instance = new PauseMenu();
        }

        return m_instance;
}

bool PauseMenu::isOptionsMenuOpen() {
	return PauseMenu::get()->m_optionsMenuOpen;
}

void PauseMenu::render(int xRes, int yRes)
{
    if (!PauseMenu::isPaused())
		return;

	// Draw pause screen overlay

	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0, 0, 0, 0.75f);
	glBegin(GL_QUADS);
		glVertex2i(0, yRes);
		glVertex2i(xRes, yRes);
		glVertex2i(xRes, -yRes);
		glVertex2i(0, -yRes);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	if (PauseMenu::isOptionsMenuOpen()) {

	}
	else {
		PauseMenu::get()->displayPauseOptionButton(PauseMenu::PauseMenuOption::RESUME, yRes - 50, 50, "Resume");
		PauseMenu::get()->displayPauseOptionButton(PauseMenu::PauseMenuOption::OPTIONS, yRes - 100, 50, "Options");
		PauseMenu::get()->displayPauseOptionButton(PauseMenu::PauseMenuOption::QUIT, yRes - 150, 50, "Quit");
	}
}

PauseMenu::PauseMenuOption PauseMenu::getSelectedOption()
{
        return PauseMenu::get()->m_selectedOption;
}

PauseMenu::PauseMenu()
{
        this->m_selectedOption = PauseMenu::PauseMenuOption::RESUME;
        m_paused = false;
        m_optionsMenuOpen = false;
		// m_config = ConfigLoader("./config/main.config");
}

void PauseMenu::pause()
{
	std::cout << "The game is now paused\n";
        PauseMenu::get()->m_paused = true;
}

void PauseMenu::resume()
{
	std::cout << "The game has been unpaused" << std::endl;
	PauseMenu::setSelectedOption(PauseMenu::PauseMenuOption::RESUME);
	PauseMenu::get()->m_paused = false;
    PauseMenu::get()->hideOptionsScreen();
}

bool PauseMenu::isPaused()
{
	return PauseMenu::get()->m_paused;
}

void PauseMenu::setSelectedOption(PauseMenu::PauseMenuOption option)
{
        PauseMenu* instance = PauseMenu::get();
        instance->m_selectedOption = option;
}

void PauseMenu::selectOption(PauseMenu::PauseMenuOption option)
{
	switch (option)
	{
		case PauseMenuOption::QUIT:
			exit(0);
			break;
		case PauseMenuOption::RESUME:
			PauseMenu::resume();
			break;
		case PauseMenuOption::OPTIONS:
			PauseMenu::get()->showOptionsScreen();
			break;
		default:
			break;
	}

}

void PauseMenu::displayPauseOptionButton(PauseMenu::PauseMenuOption correspondingOption, int bot, int left, const std::string &text)
{
	int color = correspondingOption == PauseMenu::getSelectedOption() 
		? PauseMenu::SELECTED_BUTTON_COLOR 
		: PauseMenu::BUTTON_COLOR;

    PauseMenu::get()->displayButton(bot, left, text, color);
}

void PauseMenu::displayOptionButton(PauseMenu::PauseMenuOption correspondingOption, int bot, int left, const std::string &text)
{
    // PauseMenu::get()->displayButton(bot, left, text, color);
}


void PauseMenu::displayButton(int bot, int left, const std::string &text, int color) {
	Rect r;
	r.bot = bot;
	r.left = left;
	r.center = 0;
    ggprint16(&r, 16, color, text.c_str());
}

void PauseMenu::showOptionsScreen() {
	this->m_optionsMenuOpen = true;
}

void PauseMenu::hideOptionsScreen() {
    this->m_optionsMenuOpen = false;
}
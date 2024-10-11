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

void PauseMenu::render(int xRes, int yRes)
{
        if (!PauseMenu::isPaused())
		return;

	// Draw pause screen overlay

	glPushMatrix();
	glEnable(GL_BLEND | GL_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0, 0, 0, 0.15f);
	glBegin(GL_QUADS);
		glVertex2i(0, yRes);
		glVertex2i(xRes, yRes);
		glVertex2i(xRes, -yRes);
		glVertex2i(0, -yRes);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);

	PauseMenu::get()->displayButton(PauseMenu::PauseMenuOption::RESUME, yRes - 50, 50, "Resume");
	PauseMenu::get()->displayButton(PauseMenu::PauseMenuOption::OPTIONS, yRes - 100, 50, "Options");
	PauseMenu::get()->displayButton(PauseMenu::PauseMenuOption::QUIT, yRes - 150, 50, "Quit");
}

PauseMenu::PauseMenuOption PauseMenu::getSelectedOption()
{
        return PauseMenu::get()->m_selectedOption;
}

PauseMenu::PauseMenu()
{
        m_selectedOption = PauseMenu::PauseMenuOption::RESUME;
        m_paused = false;
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
}

bool PauseMenu::isPaused()
{
	return PauseMenu::get()->m_paused;
}

void PauseMenu::setSelectedOption(PauseMenu::PauseMenuOption option)
{
        PauseMenu* instance = PauseMenu::get();
        instance->m_selectedOption = option;

        switch (option)
        {
                case PauseMenu::PauseMenuOption::OPTIONS:
                        std::cout << "We're in options" << std::endl;
                        break;
                case PauseMenu::PauseMenuOption::QUIT:
                        std::cout << "We're in quit" << std::endl;
                        break;
                case PauseMenu::PauseMenuOption::RESUME:
                        std::cout << "We're in resume" << std::endl;
                        break;
                default:
                        std::cout << "idk lol" << std::endl;
                        break;
        }
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
		default:
			break;
	}

}

void PauseMenu::displayButton(PauseMenu::PauseMenuOption correspondingOption, int bot, int left, const std::string &text)
{
	Rect r;
	r.bot = bot;
	r.left = left;
	r.center = 0;

	int color = correspondingOption == PauseMenu::getSelectedOption() 
		? PauseMenu::SELECTED_BUTTON_COLOR 
		: PauseMenu::BUTTON_COLOR;

	ggprint16(&r, 16, color, text.c_str());
}

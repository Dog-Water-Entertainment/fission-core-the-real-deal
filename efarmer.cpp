#include <iostream>
#include <set>
#include <vector>
#include <functional>
#include "fonts.h"
#include <GL/glx.h>
#include "efarmer.h"

PauseMenu* PauseMenu::m_instance = nullptr;

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

	Rect r;
	r.bot = yRes - 50;
	r.left = 10;
	r.center = 0;

	ggprint8b(&r, 16, 0x00fff44f, "Foobar");

	// Draw pause screen overlay

	glPushMatrix();
	glEnable(GL_BLEND | GL_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0, 0, 0, 0.5f);
	glBegin(GL_QUADS);
		glVertex2i(0, yRes);
		glVertex2i(xRes, yRes);
		glVertex2i(xRes, -yRes);
		glVertex2i(0, -yRes);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
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


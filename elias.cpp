#include <iostream>
#include "elias.h"

PauseMenu* PauseMenu::m_instance = nullptr;

PauseMenu* PauseMenu::get()
{
        if (m_instance == nullptr)
        {
                m_instance = new PauseMenu();
        }

        return m_instance;
}

void PauseMenu::render()
{
        // TODO: Add rendering logic.
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
        PauseMenu::get()->m_paused = true;
        // Do pause stuff.
}

void PauseMenu::resume()
{
        PauseMenu::get()->m_paused = false;
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
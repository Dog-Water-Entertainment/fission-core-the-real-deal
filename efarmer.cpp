#include <iostream>
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
	std::cout << "The game is now paused\n";
        PauseMenu::get()->m_paused = true;
        // Do pause stuff.
}

void PauseMenu::resume()
{
	std::cout << "The game has been unpaused" << std::endl;
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

InputManager* InputManager::m_instance = nullptr;

InputManager::InputManager()
{
	pressedKeys = std::set<int>{};
}

InputManager* InputManager::get()
{
	if (m_instance == nullptr)
		m_instance = new InputManager();

	return m_instance;
}

void InputManager::render()
{

}

void InputManager::registerInputPressedEvent()
{
}

void InputManager::registerInputTriggerEvent()
{
}

bool InputManager::isKeyTriggered(int key)
{
}

bool InputManager::isKeyPressed(int key)
{
	InputManager* instance = InputManager::get();

	if (auto search = instance->pressedKeys.find(key); search != instance->pressedKeys.end())
		return true;

	return false;
}


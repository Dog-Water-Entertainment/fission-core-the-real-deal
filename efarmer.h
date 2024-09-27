#include <iostream>
#include <set>

#ifndef EFARMER_H
#define EFARMER_H

class PauseMenu
{
public:
	enum class PauseMenuOption
	{
		RESUME,
		OPTIONS,
		QUIT,
		NUM_PAUSE_MENU
	};

	static PauseMenu* get();
	static void render();
	static PauseMenu::PauseMenuOption getSelectedOption();
	static void setSelectedOption(PauseMenu::PauseMenuOption);
	static bool isPaused();
	static void pause();
	static void resume();

private:
	PauseMenu();
	PauseMenu::PauseMenuOption m_selectedOption;
	bool m_paused;
	
	static PauseMenu* m_instance;
};

class InputManager
{
public:
	static bool isKeyPressed(int key);
	static bool isKeyTriggered(int key);
	static void registerInputTriggerEvent();
	static void registerInputPressedEvent();
	static void render();
	static InputManager* get(); 

private:
	InputManager();
	
	static InputManager* m_instance;
	std::set<int> pressedKeys;

};


#endif

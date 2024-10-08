#include <iostream>
#include <set>
#include <vector>
#include <functional>

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
	static void render(int xRes, int yRes);
	static PauseMenu::PauseMenuOption getSelectedOption();
	static void setSelectedOption(PauseMenu::PauseMenuOption);
	static bool isPaused();
	static void pause();
	static void resume();
	static void selectOption(PauseMenu::PauseMenuOption);
private:
	PauseMenu();
	PauseMenu::PauseMenuOption m_selectedOption;
	bool m_paused;
	
	static PauseMenu* m_instance;
};
#endif

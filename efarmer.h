#include <iostream>
#include <set>
#include <vector>
#include <functional>
#include <string>

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

	static const int SELECTED_BUTTON_COLOR;
	static const int BUTTON_COLOR;

private:
	PauseMenu();
	void displayButton(PauseMenu::PauseMenuOption correspondingOption, int bot, int left, const std::string &text);
	PauseMenu::PauseMenuOption m_selectedOption;
	bool m_paused;
	static PauseMenu* m_instance;
};
#endif

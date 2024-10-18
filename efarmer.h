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

	enum class Option {
		MUTE_MUSIC,
		MUTE_SOUNDS,
		DISPLAY_FPS,
	};

	static PauseMenu* get();
	static void render(int xRes, int yRes);
	static PauseMenu::PauseMenuOption getSelectedOption();
	static void setSelectedOption(PauseMenu::PauseMenuOption);
	static bool isPaused();
	static void pause();
	static void resume();
	static bool isOptionsMenuOpen();
	static void selectOption(PauseMenu::PauseMenuOption);

	static const int SELECTED_BUTTON_COLOR;
	static const int BUTTON_COLOR;

private:
	PauseMenu();
	
	void displayButton(int bot, int left, const std::string& text, int color);
	void displayPauseOptionButton(PauseMenu::PauseMenuOption option, int bot, int left, const std::string& text);
	void displayOptionButton(PauseMenu::PauseMenuOption option, int bot, int left, const std::string& text);

	void showOptionsScreen();
	void hideOptionsScreen();

	PauseMenu::PauseMenuOption m_selectedOption;
	bool m_paused;
	// ConfigLoader m_config;
	bool m_optionsMenuOpen;
	static PauseMenu* m_instance;
};
#endif

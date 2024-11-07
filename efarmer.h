#include <iostream>
#include <set>
#include <vector>
#include <functional>
#include <string>
#include <map>
#include "utils/Config.h"

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

	enum class Setting {
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
	static PauseMenu::Setting getSelectedSetting();
	static float getSettingValue(PauseMenu::Setting setting);

	static const int SELECTED_BUTTON_COLOR;
	static const int BUTTON_COLOR;

private:
	PauseMenu();
	
	void displayButton(int bot, int left, const std::string& text, int color);
	void displayPauseOptionButton(PauseMenu::PauseMenuOption option, int bot, int left, const std::string& text);
	void setSelectedSetting(PauseMenu::Setting setting);
	void displaySettingButton(PauseMenu::Setting setting, int bot, int left, const std::string& text);

	void showOptionsScreen();
	void hideOptionsScreen();

	PauseMenu::Setting selectedSetting;
	PauseMenu::PauseMenuOption m_selectedOption;

	bool m_paused;
	ConfigLoader m_config = ConfigLoader("./config/settings.config");

	bool m_optionsMenuOpen;
	static PauseMenu* m_instance;

	const std::map<PauseMenu::Setting, float> defaultSettingValues;
	const std::map<PauseMenu::Setting, std::string> settingKeys;
};
#endif

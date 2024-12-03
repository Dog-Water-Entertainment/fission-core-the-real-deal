#include <iostream>
#include <set>
#include <vector>
#include <functional>
#include <string>
#include <map>
#include "Config.h"
#include <array>
#include <AL/al.h>
#include <AL/alc.h>

#ifndef EFARMER_H
#define EFARMER_H

class Termination {
public:
	static bool IsTerminated();
	static void Terminate();
	static Termination* GetInstance();
private:
	Termination();

	static Termination* instance;
	bool isTerminated;
};

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
		SETTING_NULL = -1,
		DISPLAY_FPS,
	};

	enum class SettingButton {
		DISPLAY_FPS,
		BACK,
	};

	enum class State {
		CLOSED,
		HOME,
		SETTINGS,
	};

	static PauseMenu* get();
	static void render(int xRes, int yRes);
	static PauseMenu::PauseMenuOption getSelectedOption();
	static void setSelectedOption(PauseMenu::PauseMenuOption);
	static PauseMenu::State getState();
	static void setState(PauseMenu::State state);
	static void pause();
	static void resume();
	static void selectOption(PauseMenu::PauseMenuOption option);
	static PauseMenu::SettingButton getSelectedSetting();
	static float getSettingValue(PauseMenu::Setting setting);
	static void setSelectedSetting(PauseMenu::SettingButton setting);
	static bool isPaused();
	static void toggleSetting(PauseMenu::SettingButton setting);
	static void initialize();

	static const int SELECTED_BUTTON_COLOR;
	static const int BUTTON_COLOR;

	static const std::map<PauseMenu::Setting, float> defaultSettingValues;
	static const std::map<PauseMenu::Setting, std::string> settingKeys;
	static const std::map<PauseMenu::SettingButton, PauseMenu::Setting> settingButtonMap;
	static const std::map<PauseMenu::Setting, float> settingValues;

private:
	PauseMenu();
	
	void displayButton(int bot, int left, const std::string& text, int color);
	void displayPauseOptionButton(PauseMenu::PauseMenuOption option, int bot, int left, const std::string& text);
	void displaySettingButton(PauseMenu::SettingButton setting, int bot, int left, const std::string& text);

	PauseMenu::PauseMenuOption m_selectedOption;
	PauseMenu::State state;
	PauseMenu::SettingButton selectedSetting;

	ConfigLoader m_config = ConfigLoader("./config/settings.config");
	bool initialized;
	static PauseMenu* m_instance;
};

class DialogManager {
public:
	static void promptDialog(std::string speaker, std::vector<std::string> dialog, int x, int y, int dialogColor);
	static bool isDialogActive();
	static void render();
	static void tryAdvanceDialog(bool force);

private:
	DialogManager();

	static DialogManager* instance;
	static DialogManager* getInstance();

	bool dialogActive;
	std::vector<std::string> current_dialog;
	int current_dialog_index;
	int current_character_index;
	int x_pos;
	int y_pos;
	int dialog_color;
	std::string speaker;
	double time_prompted;
};

class AudioManager {
public:
	static AudioManager* getInstance();
	static void initialize();
private:
	AudioManager();

	static AudioManager* instance;

	bool initialized;
	ALCdevice* device;
};

#endif

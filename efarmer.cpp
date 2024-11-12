#include <iostream>
#include <set>
#include <vector>
#include <functional>
#include "fonts.h"
#include <GL/glx.h>
#include <string>
#include "efarmer.h"
#include <map>
#include "termination.h"

PauseMenu* PauseMenu::m_instance = nullptr;
const int PauseMenu::SELECTED_BUTTON_COLOR = 0x0000FF00;
const int PauseMenu::BUTTON_COLOR = 0x00FFFF00;
const std::map<PauseMenu::Setting, float> defaultSettingValues{
	{PauseMenu::Setting::DISPLAY_FPS, 0},
	{PauseMenu::Setting::MUTE_MUSIC, 0},
	{PauseMenu::Setting::MUTE_SOUNDS, 0}
};
const std::map<PauseMenu::Setting, std::string> settingKeys{
	{PauseMenu::Setting::DISPLAY_FPS, "displayFPS"},
	{PauseMenu::Setting::MUTE_MUSIC, "muteMusic"},
	{PauseMenu::Setting::MUTE_SOUNDS, "muteSounds"},
};

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
	PauseMenu::State state = PauseMenu::getState();

	if (state == PauseMenu::State::CLOSED)
		return;

	// Draw pause screen overlay

	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0, 0, 0, 0.75f);
	glBegin(GL_QUADS);
		glVertex2i(0, yRes);
		glVertex2i(xRes, yRes);
		glVertex2i(xRes, -yRes);
		glVertex2i(0, -yRes);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	PauseMenu* instance = PauseMenu::get();

	using Option = PauseMenu::PauseMenuOption;
	using Setting = PauseMenu::Setting;

	switch (state) {
		case PauseMenu::State::HOME: {
			instance->displayPauseOptionButton(Option::RESUME, yRes - 50, 50, "Resume");
			instance->displayPauseOptionButton(Option::OPTIONS, yRes - 100, 50, "Options");
			instance->displayPauseOptionButton(Option::QUIT, yRes - 150, 50, "Quit");
			break;
		}
		case PauseMenu::State::SETTINGS: {
			instance->displaySettingButton(Setting::DISPLAY_FPS, yRes - 50, 50, "Toggle FPS");
			break;
		}
		default: {
			break;
		}
	}
}

PauseMenu::PauseMenuOption PauseMenu::getSelectedOption()
{
	return PauseMenu::get()->m_selectedOption;
}

PauseMenu::PauseMenu()
{
	this->selectedSetting = PauseMenu::Setting::DISPLAY_FPS;
	this->m_selectedOption = PauseMenu::PauseMenuOption::RESUME;
	this->state = PauseMenu::State::CLOSED;
	this->m_optionsMenuOpen = false;
}

void PauseMenu::pause()
{
	std::cout << "The game is now paused\n";
	PauseMenu::get()->state = PauseMenu::State::HOME;
}

void PauseMenu::resume()
{
	std::cout << "The game has been unpaused" << std::endl;
	PauseMenu::setSelectedOption(PauseMenu::PauseMenuOption::RESUME);
	PauseMenu::setState(PauseMenu::State::CLOSED);
	PauseMenu::get()->hideOptionsScreen();
}

void PauseMenu::setSelectedOption(PauseMenu::PauseMenuOption option)
{
	PauseMenu* instance = PauseMenu::get();
	instance->m_selectedOption = option;
}

void PauseMenu::selectOption(PauseMenu::PauseMenuOption option)
{
	switch (option)
	{
		case PauseMenuOption::QUIT:
			Termination::Terminate();
			break;
		case PauseMenuOption::RESUME:
			PauseMenu::resume();
			break;
		case PauseMenuOption::OPTIONS:
			PauseMenu::get()->showOptionsScreen();
			break;
		default:
			break;
	}
}

void PauseMenu::setSelectedSetting(PauseMenu::Setting setting) {
	PauseMenu::get()->selectedSetting = setting;
}

void PauseMenu::displayPauseOptionButton(PauseMenu::PauseMenuOption correspondingOption, int bot, int left, const std::string &text)
{
	int color = correspondingOption == PauseMenu::getSelectedOption() 
		? PauseMenu::SELECTED_BUTTON_COLOR 
		: PauseMenu::BUTTON_COLOR;
	
	PauseMenu::get()->displayButton(bot, left, text, color);
}

void PauseMenu::displaySettingButton(PauseMenu::Setting setting, int bot, int left, const std::string &text)
{
	int color = setting == PauseMenu::getSelectedSetting() 
		? PauseMenu::SELECTED_BUTTON_COLOR 
		: PauseMenu::BUTTON_COLOR;

	PauseMenu::get()->displayButton(bot, left, text, color);
}

PauseMenu::Setting PauseMenu::getSelectedSetting() {
	return PauseMenu::get()->selectedSetting;
}

void PauseMenu::displayButton(int bot, int left, const std::string &text, int color) {
	Rect r;
	r.bot = bot;
	r.left = left;
	r.center = 0;
	ggprint16(&r, 16, color, text.c_str());
}

void PauseMenu::showOptionsScreen() {
	this->m_optionsMenuOpen = true;
}

void PauseMenu::hideOptionsScreen() {
	this->m_optionsMenuOpen = false;
}

float PauseMenu::getSettingValue(PauseMenu::Setting setting) {
	return get()->m_config.getFloat(get()->settingKeys.at(setting), get()->defaultSettingValues.at(setting));
}

void PauseMenu::setState(PauseMenu::State state) {
	get()->state = state;
}

PauseMenu::State PauseMenu::getState() {
	return get()->state;
}

bool PauseMenu::isPaused() {
	return get()->state != PauseMenu::State::CLOSED;
}
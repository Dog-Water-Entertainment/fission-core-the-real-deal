#include <iostream>
#include <set>
#include <vector>
#include <functional>
#include "fonts.h"
#include <GL/glx.h>
#include <string>
#include "efarmer.h"
#include "TimeUtils.h"
#include <map>
#include "Bckgr.h"
#include <cmath>
#include <AL/al.h>
#include <AL/alc.h>

AudioManager* AudioManager::instance = nullptr;

PauseMenu* PauseMenu::m_instance = nullptr;
const int PauseMenu::SELECTED_BUTTON_COLOR = 0x0000FF00;
const int PauseMenu::BUTTON_COLOR = 0x00FFFF00;
const std::map<PauseMenu::Setting, float> PauseMenu::defaultSettingValues{
	{PauseMenu::Setting::DISPLAY_FPS, 0},
};
const std::map<PauseMenu::Setting, std::string> PauseMenu::settingKeys{
	{PauseMenu::Setting::DISPLAY_FPS, "displayFPS"},
};

const std::map<PauseMenu::SettingButton, PauseMenu::Setting> PauseMenu::settingButtonMap{
	{PauseMenu::SettingButton::DISPLAY_FPS, PauseMenu::Setting::DISPLAY_FPS},
	{PauseMenu::SettingButton::BACK, PauseMenu::Setting::SETTING_NULL},
};

DialogManager* DialogManager::instance = nullptr;

PauseMenu* PauseMenu::get()
{
	if (m_instance == nullptr)
		m_instance = new PauseMenu();

	return m_instance;
}

void PauseMenu::initialize() 
{
	PauseMenu* instance = PauseMenu::get();

	if (instance->initialized) {
		return;
	}

	instance->initialized = true;
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

	switch (state) {
		case PauseMenu::State::HOME: {
			instance->displayPauseOptionButton(
				Option::RESUME, 
				yRes - 50, 
				50, 
				"Resume"
			);
			instance->displayPauseOptionButton(
				Option::OPTIONS, 
				yRes - 100, 
				50, 
				"Options"
			);
			instance->displayPauseOptionButton(
				Option::QUIT, 
				yRes - 150, 
				50, 
				"Quit"
			);
			break;
		}
		case PauseMenu::State::SETTINGS: {
			instance->displaySettingButton(
				SettingButton::DISPLAY_FPS, 
				yRes - 50, 
				50, 
				"Toggle FPS"
			);
			instance->displaySettingButton(
				SettingButton::BACK, 
				yRes - 100, 
				50, 
				"Back"
			);
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
	this->selectedSetting = PauseMenu::SettingButton::DISPLAY_FPS;
	this->m_selectedOption = PauseMenu::PauseMenuOption::RESUME;
	this->state = PauseMenu::State::CLOSED;
}

void PauseMenu::pause()
{
	PauseMenu::get()->state = PauseMenu::State::HOME;
}

void PauseMenu::resume()
{
	PauseMenu::setSelectedOption(PauseMenu::PauseMenuOption::RESUME);
	PauseMenu::setState(PauseMenu::State::CLOSED);
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
			PauseMenu::setState(PauseMenu::State::SETTINGS);
			break;
		default:
			break;
	}
}

void PauseMenu::setSelectedSetting(PauseMenu::SettingButton setting)
{
	PauseMenu::get()->selectedSetting = setting;
}

void PauseMenu::displayPauseOptionButton(
	PauseMenu::PauseMenuOption correspondingOption, 
	int bot, 
	int left, 
	const std::string &text
)
{
	int color = correspondingOption == PauseMenu::getSelectedOption() 
		? PauseMenu::SELECTED_BUTTON_COLOR 
		: PauseMenu::BUTTON_COLOR;
	
	PauseMenu::get()->displayButton(bot, left, text, color);
}

void PauseMenu::displaySettingButton(
	PauseMenu::SettingButton settingButton, 
	int bot, 
	int left, 
	const std::string &text
)
{
	int color = settingButton == PauseMenu::getSelectedSetting() 
		? PauseMenu::SELECTED_BUTTON_COLOR 
		: PauseMenu::BUTTON_COLOR;

	std::string textToDraw = "";

	if (settingButton != PauseMenu::SettingButton::BACK) {
		PauseMenu::Setting setting = PauseMenu::settingButtonMap.at(settingButton); 
		if (PauseMenu::get()->getSettingValue(setting) == 1)
			textToDraw = text + ": On";
		else
			textToDraw = text + ": Off";
	}
	else {
		textToDraw = text;
	}
	
	PauseMenu::get()->displayButton(bot, left, textToDraw, color);
}

PauseMenu::SettingButton PauseMenu::getSelectedSetting() 
{
	return PauseMenu::get()->selectedSetting;
}

void PauseMenu::displayButton(
	int bot, 
	int left, 
	const std::string &text, 
	int color
) {
	Rect r;
	r.bot = bot;
	r.left = left;
	r.center = 0;
	ggprint16(&r, 16, color, text.c_str());
}

float PauseMenu::getSettingValue(PauseMenu::Setting setting)
{
	PauseMenu* instance = PauseMenu::get();

	return instance->m_config.getFloat(
		PauseMenu::settingKeys.at(setting), 
		PauseMenu::defaultSettingValues.at(setting)
	);
}

void PauseMenu::setState(PauseMenu::State state) 
{
	get()->state = state;
}

PauseMenu::State PauseMenu::getState() 
{
	return get()->state;
}

bool PauseMenu::isPaused() 
{
	return get()->state != PauseMenu::State::CLOSED;
}

void PauseMenu::toggleSetting(PauseMenu::SettingButton settingButton)
{
	PauseMenu* instance = PauseMenu::get();

	if (settingButton == PauseMenu::SettingButton::BACK) {
		PauseMenu::setSelectedSetting(
			PauseMenu::SettingButton::DISPLAY_FPS
			);
		PauseMenu::setState(PauseMenu::State::HOME);
	}
	else {
		auto setting = PauseMenu::settingButtonMap.at(settingButton);
		std::string key = instance->settingKeys.at(setting);
		float value = instance->m_config.getFloat(key);

		if (value == 0) {
			instance->m_config.setFloat(key, 1);
		}
		else {
			instance->m_config.setFloat(key, 0);
		}
	}
}

Termination* Termination::instance;

Termination::Termination() 
{
	this->isTerminated = false;
}

Termination* Termination::GetInstance() 
{
	if (Termination::instance == nullptr)
		Termination::instance = new Termination();

	return Termination::instance;
}

void Termination::Terminate() 
{
	Termination::GetInstance()->isTerminated = true;
}

bool Termination::IsTerminated() 
{
	return Termination::GetInstance()->isTerminated;
}

DialogManager::DialogManager() 
{
	this->dialogActive = false;
	this->current_dialog = {};
	this->current_dialog_index = 0;
	this->current_character_index = 0;
	this->x_pos = 0;
	this->y_pos = 0;
	this->time_prompted = 0;
}

void DialogManager::promptDialog(
	std::string speaker, 
	std::vector<std::string> dialog, 
	int x, 
	int y, 
	int dialog_color
) {
	DialogManager* instance = DialogManager::getInstance();

	if (DialogManager::isDialogActive()) {
		return;
	}

	instance->current_dialog_index = 0;
	instance->dialogActive = true;
	instance->current_dialog = dialog;
	instance->x_pos = x;
	instance->y_pos = y;
	instance->dialog_color = dialog_color;
	instance->speaker = speaker;
	instance->time_prompted = TimeUtils::get_time();
}

DialogManager* DialogManager::getInstance() 
{
	if (!DialogManager::instance) {
		DialogManager::instance = new DialogManager();
	}

	return DialogManager::instance;
}

bool DialogManager::isDialogActive() 
{
	return DialogManager::getInstance()->dialogActive;
}

void DialogManager::render() 
{
	if (!DialogManager::isDialogActive()) {
		return;
	}
	
	Bckgr::dialoguebackground();

	DialogManager* instance = DialogManager::getInstance();

	std::string current_text = 
		instance->current_dialog.at(instance->current_dialog_index);

	double current_time = TimeUtils::get_time();
	double time_difference = 
		(current_time - instance->time_prompted) / 1000;
	
	/**
	 * Do not update the character index in case the user skipped the 
	 * typewriter effect.
	 */
	if ((int)current_text.size() - 1 != instance->current_character_index) {
		instance->current_character_index = 
			std::min(
				static_cast<double>(current_text.size() - 1),
				time_difference/0.01
			);		
	}

	std::string text_to_render = 
		"* " 
		+ current_text.substr(0, instance->current_character_index + 1);

	Rect r;
	r.bot = instance->y_pos;
	r.left = instance->x_pos;
	r.center = 0;
	ggprint16(&r, 16, instance->dialog_color, text_to_render.c_str());
}

void DialogManager::tryAdvanceDialog(bool force) 
{
	if (!DialogManager::isDialogActive()) {
		return;
	}

	DialogManager* instance = DialogManager::getInstance();

	std::string current_text = 
		instance->current_dialog.at(instance->current_dialog_index);
	int text_size = current_text.size();
	int last_dialog_index = instance->current_dialog.size() - 1;

	if (instance->current_character_index == text_size - 1 || force) {
		if (instance->current_dialog_index == last_dialog_index) {
			instance->dialogActive = false;
		}
		else {
			instance->current_dialog_index++;
			instance->current_character_index = 0;
			instance->time_prompted = TimeUtils::get_time();
		}
	}
	else {
		instance->current_character_index = current_text.size() - 1;
	}
}

AudioManager::AudioManager() {
	this->initialized = false;
	this->device = alcOpenDevice(NULL);

	if (!device) {
		std::cout << "Failed to open default device!\n";
	}
}

AudioManager* AudioManager::getInstance() {
	if (AudioManager::instance == nullptr) {
		AudioManager::instance = new AudioManager();
	}

	return AudioManager::instance;
}

void AudioManager::initialize() {
	AudioManager::getInstance()->initialized = true;
}
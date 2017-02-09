#pragma once

#include "GuiComponent.h"
#include "components/MenuComponent.h"
#include "components/OptionListComponent.h"
#include <functional>
#include <Window.h>
#include <SystemData.h>
#include "guis/GuiSettings.h"
#include "guis/GuiMenu.h"


class RecalboxGuiMenu : public GuiMenu
{
public:
	RecalboxGuiMenu(Window* window);
	~RecalboxGuiMenu();

protected:
	void createConfigInput();
	void createInputTextRow(GuiSettings * gui, std::string title, const char* settingsID, bool password);

private:
	MenuComponent mMenu;
	TextComponent mVersion;


	std::shared_ptr<OptionListComponent<std::string>> createRatioOptionList(Window *window,
																			std::string configname) const;

	void popSystemConfigurationGui(SystemData *systemData, std::string previouslySelectedEmulator) const;

	std::vector<StrInputConfig*> mLoadedInput; // used to keep information about loaded devices in case there are unpluged between device window load and save
};

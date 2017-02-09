#pragma once

#include "GuiComponent.h"
#include "components/MenuComponent.h"
#include "components/OptionListComponent.h"
#include <functional>
#include <Window.h>
#include <SystemData.h>
#include "guis/GuiSettings.h"

class StrInputConfig
{
 public:
  StrInputConfig(std::string ideviceName, std::string ideviceGUIDString) {
    deviceName = ideviceName;
    deviceGUIDString = ideviceGUIDString;
  }

  std::string deviceName;
  std::string deviceGUIDString;
};

class GuiMenu : public GuiComponent
{
public:
	GuiMenu(Window* window);
	~GuiMenu();

	bool input(InputConfig* config, Input input) override;
	void onSizeChanged() override;
	std::vector<HelpPrompt> getHelpPrompts() override;

protected:
	void addEntry(const char* name, unsigned int color, bool add_arrow, const std::function<void()>& func);
	void createInputTextRow(GuiSettings * gui, std::string title, const char* settingsID, bool password);
	void clearLoadedInput();
	void popSystemConfigurationGui(SystemData *systemData, std::string previouslySelectedEmulator) const;
	void createConfigInput();

private:
	MenuComponent mMenu;
	TextComponent mVersion;

	std::shared_ptr<OptionListComponent<std::string>> createRatioOptionList(Window *window,
                                                                        std::string configname) const;

	std::vector<StrInputConfig*> mLoadedInput; // used to keep information about loaded devices in case there are unpluged between device window load and save
};

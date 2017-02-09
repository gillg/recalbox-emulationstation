#pragma once
#include <string>
#include <map>
#include "SettingsManager.h"

//This is a singleton for storing settings.
class Settings: public SettingsManager
{
public:
	static Settings* getInstance();

	bool load();
	void loadFile();

    bool save();
	void saveFile();

protected:
    //Clear everything and load default values.
    void setDefaults();

private:
	static Settings* sInstance;

	Settings();

};

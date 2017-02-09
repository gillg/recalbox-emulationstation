//
// Created by gillg on 06/02/17.
//

#include "SettingsManager.h"
#include "Log.h"
#include "RecalboxConf.h"
#include "Settings.h"

SettingsManager *SettingsManager::sInstance = NULL;

SettingsManager::SettingsManager() {
}

void SettingsManager::init(const std::string &type) {
    if (sInstance == NULL) {
        if (type == "RecalboxConf") {
            sInstance = RecalboxConf::getInstance();
        } else {
            sInstance = Settings::getInstance();
        }
    }
}

SettingsManager *SettingsManager::getInstance() {
    init("Settings");
    return sInstance;
}

std::string SettingsManager::get(const std::string &name) {
    return getString(name);
}

std::string SettingsManager::get(const std::string &name, const std::string &defaut) {
    std::string value = get(name);
    if (value == "") {
        return defaut;
    }
    return value;
}

void SettingsManager::set(const std::string &name, const std::string &value) {
    setString(name, value);
}

//Print a warning message if the setting we're trying to get doesn't already exist in the map, then return the value in the map.
#define SETTINGS_GETSET(type, mapName, getMethodName, setMethodName) type SettingsManager::getMethodName(const std::string& name) \
{ \
    if(mapName.find(name) == mapName.end()) \
    { \
        LOG(LogError) << "Tried to use unset setting " << name << "!"; \
    } \
    return mapName[name]; \
} \
void SettingsManager::setMethodName(const std::string& name, type value) \
{ \
    mapName[name] = value; \
}

SETTINGS_GETSET(bool, mBoolMap, getBool, setBool);

SETTINGS_GETSET(int, mIntMap, getInt, setInt);

SETTINGS_GETSET(float, mFloatMap, getFloat, setFloat);

SETTINGS_GETSET(const std::string&, mStringMap, getString, setString);
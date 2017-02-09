//
// Created by gillg on 06/02/17.
//

#ifndef EMULATIONSTATION_ALL_SETTINGSMANAGER_H
#define EMULATIONSTATION_ALL_SETTINGSMANAGER_H

#include <string>
#include <map>


class SettingsManager {

public:
    static SettingsManager* getInstance();

    static void init(const std::string &type);

    virtual bool load() {};

    virtual bool save() {};

    std::string get(const std::string &name);
    std::string get(const std::string &name, const std::string &defaut);

    void set(const std::string &name, const std::string &value);

    //You will get a warning if you try a get on a key that is not already present.
    bool getBool(const std::string& name);
    int getInt(const std::string& name);
    float getFloat(const std::string& name);
    const std::string& getString(const std::string& name);

    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setString(const std::string& name, const std::string& value);

protected:
    SettingsManager();

    //Clear everything and load default values.
    virtual void setDefaults() {};

    std::map<std::string, bool> mBoolMap;
    std::map<std::string, int> mIntMap;
    std::map<std::string, float> mFloatMap;
    std::map<std::string, std::string> mStringMap;

private:
    static SettingsManager* sInstance;
};


#endif //EMULATIONSTATION_ALL_SETTINGSMANAGER_H

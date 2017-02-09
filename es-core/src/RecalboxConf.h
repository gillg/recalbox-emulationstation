//
// Created by matthieu on 12/09/15.
//

#ifndef EMULATIONSTATION_ALL_RECALBOXCONF_H
#define EMULATIONSTATION_ALL_RECALBOXCONF_H


#include <string>
#include <map>
#include "SettingsManager.h"

class RecalboxConf: public SettingsManager {

public:
    bool load();

    bool save();

    static RecalboxConf *getInstance();

protected:
    //Clear everything and load default values.
    void setDefaults();

private:
    RecalboxConf();

    static RecalboxConf *sInstance;
};


#endif //EMULATIONSTATION_ALL_RECALBOXCONF_H

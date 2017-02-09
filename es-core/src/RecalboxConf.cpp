#include "RecalboxConf.h"
#include <iostream>
#include <fstream>
#include <boost/regex.hpp>
#include "Log.h"
#include <boost/algorithm/string/predicate.hpp>

RecalboxConf *RecalboxConf::sInstance = NULL;
boost::regex validLine("^(?<key>[^;|#].*?)=(?<val>.*?)$");
boost::regex commentLine("^;(?<key>.*?)=(?<val>.*?)$");

std::string recalboxConfFile = "/recalbox/share/system/recalbox.conf";
std::string recalboxConfFileTmp = "/recalbox/share/system/recalbox.conf.tmp";

void RecalboxConf::setDefaults() {
    mBoolMap.clear();
    mIntMap.clear();

    mBoolMap["BackgroundJoystickInput"] = false;
    mBoolMap["ParseGamelistOnly"] = false;
    mBoolMap["DrawFramerate"] = false;
    mBoolMap["ShowExit"] = true;
    mBoolMap["Windowed"] = false;
    mBoolMap["UseOSK"] = true;
    mBoolMap["HideSystemView"] = false;

#ifdef _RPI_
    // don't enable VSync by default on the Pi, since it already
	// has trouble trying to render things at 60fps in certain menus
	mBoolMap["VSync"] = false;
#else
    mBoolMap["VSync"] = true;
#endif

    mBoolMap["ShowHelpPrompts"] = true;
    mBoolMap["ScrapeRatings"] = true;
    mBoolMap["IgnoreGamelist"] = false;
    mBoolMap["HideConsole"] = true;
    mBoolMap["QuickSystemSelect"] = true;
    mBoolMap["FavoritesOnly"] = false;
    mBoolMap["ShowHidden"] = false;

    mBoolMap["Debug"] = false;
    mBoolMap["DebugGrid"] = false;
    mBoolMap["DebugText"] = false;


    mBoolMap["Overscan"] = false;

    mIntMap["ScreenSaverTime"] = 5 * 60 * 1000; // 5 minutes
    mIntMap["ScraperResizeWidth"] = 400;
    mIntMap["ScraperResizeHeight"] = 0;
    mIntMap["SystemVolume"] = 96;

    mStringMap["TransitionStyle"] = "fade";
    mStringMap["ThemeSet"] = "";
    mStringMap["ScreenSaverBehavior"] = "dim";
    mStringMap["Scraper"] = "Screenscraper";
    mStringMap["Lang"] = "en_US";
    mStringMap["INPUT P1"] = "DEFAULT";
    mStringMap["INPUT P2"] = "DEFAULT";
    mStringMap["INPUT P3"] = "DEFAULT";
    mStringMap["INPUT P4"] = "DEFAULT";
    mStringMap["Overclock"] = "none";
    mStringMap["UpdateCommand"] = "/recalbox/scripts/recalbox-upgrade.sh";
    mStringMap["UpdateServer"] = "archive.recalbox.com";
    mStringMap["VersionFile"] = "/recalbox/recalbox.version";
    mStringMap["SharePartition"] = "/recalbox/share/";
    mStringMap["RecalboxSettingScript"] = "/recalbox/scripts/recalbox-config.sh";
    mStringMap["LastChangelog"] = "/recalbox/share/system/recalbox.changelog.done";
    mStringMap["Changelog"] = "/recalbox/recalbox.changelog";
    mStringMap["MusicDirectory"] = "/recalbox/share/music/";

}

RecalboxConf::RecalboxConf() {
    setDefaults();
    load();
}

RecalboxConf *RecalboxConf::getInstance() {
    if (sInstance == NULL)
        sInstance = new RecalboxConf();

    return sInstance;
}

bool RecalboxConf::load() {
    std::string line;
    std::ifstream recalboxConf(recalboxConfFile);
    if (recalboxConf && recalboxConf.is_open()) {
        while (std::getline(recalboxConf, line)) {
            boost::smatch lineInfo;
            if (boost::regex_match(line, lineInfo, validLine)) {
                set(lineInfo["key"], lineInfo["val"]);
            }
        }
        recalboxConf.close();
    } else {
        LOG(LogError) << "Unable to open " << recalboxConfFile;
        return false;
    }
    return true;
}

bool RecalboxConf::save() {
    std::ifstream filein(recalboxConfFile); //File to read from
    if (!filein) {
        LOG(LogError) << "Unable to open for saving :  " << recalboxConfFile << "\n";
        return false;
    }
    /* Read all lines in a vector */
    std::vector<std::string> fileLines;
    std::string line;
    while (std::getline(filein, line)) {
        fileLines.push_back(line);
    }
    filein.close();

    /* Save new value if exists */
    for (std::map<std::string, std::string>::iterator it = mStringMap.begin(); it != mStringMap.end(); ++it) {
        std::string key = it->first;
        std::string val = it->second;
        bool lineFound = false;
        for (int i = 0; i < fileLines.size(); i++) {
            std::string currentLine = fileLines[i];

            if (boost::starts_with(currentLine, key+"=") || boost::starts_with(currentLine, ";"+key+"=")){
                fileLines[i] = key + "=" + val;
                lineFound = true;
            }
        }
        if(!lineFound){
            fileLines.push_back(key + "=" + val);
        }
    }
    std::ofstream fileout(recalboxConfFileTmp); //Temporary file
    if (!fileout) {
        LOG(LogError) << "Unable to open for saving :  " << recalboxConfFileTmp << "\n";
        return false;
    }
    for (int i = 0; i < fileLines.size(); i++) {
        fileout << fileLines[i] << "\n";
    }

    fileout.close();

    /* Copy back the tmp to recalbox.conf */
    std::ifstream  src(recalboxConfFileTmp, std::ios::binary);
    std::ofstream  dst(recalboxConfFile,   std::ios::binary);
    dst << src.rdbuf();

    remove(recalboxConfFileTmp.c_str());

    return true;
}

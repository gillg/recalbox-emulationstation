#include "Settings.h"
#include "Log.h"
#include "pugixml/pugixml.hpp"
#include "platform.h"
#include <boost/filesystem.hpp>
#include <boost/assign.hpp>

Settings *Settings::sInstance = NULL;

std::string confFile = getHomePath() + "/.emulationstation/es_settings.cfg";

Settings::Settings() {
    setDefaults();
    load();
}

Settings *Settings::getInstance() {
    if (sInstance == NULL)
        sInstance = new Settings();

    return sInstance;
}

void Settings::setDefaults() {
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
    mStringMap["UpdateCommand"] = "./scripts/emulationstation-upgrade.sh";
    mStringMap["UpdateServer"] = "";
    mStringMap["MusicDirectory"] = "./music/";

}

template<typename K, typename V>
void saveMap(pugi::xml_node &node, std::map<K, V> &map, const char *type) {
    for (auto iter = map.begin(); iter != map.end(); iter++) {
        pugi::xml_node parent_node = node.append_child(type);
        parent_node.append_attribute("name").set_value(iter->first.c_str());
        parent_node.append_attribute("value").set_value(iter->second);
    }
}

void Settings::saveFile() {
    save();
}

bool Settings::save() {
    pugi::xml_document doc;

    pugi::xml_node config = doc.append_child("config");
    
    saveMap<std::string, bool>(config, mBoolMap, "bool");
    saveMap<std::string, int>(config, mIntMap, "int");
    saveMap<std::string, float>(config, mFloatMap, "float");

    //saveMap<std::string, std::string>(config, mStringMap, "string");
    for (auto iter = mStringMap.begin(); iter != mStringMap.end(); iter++) {
        pugi::xml_node node = config.append_child("string");
        node.append_attribute("name").set_value(iter->first.c_str());
        node.append_attribute("value").set_value(iter->second.c_str());
    }

    return doc.save_file(confFile.c_str());
}

void Settings::loadFile() {
    load();
}

bool Settings::load() {
    if (!boost::filesystem::exists(confFile))
        return false;

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(confFile.c_str());
    if (!result) {
        LOG(LogError) << "Could not parse Settings file!\n   " << result.description();
        return false;
    }

    pugi::xml_node config = doc.child("config");
    if(config) { /* correct file format, having a config root node */
      for (pugi::xml_node node = config.child("bool"); node; node = node.next_sibling("bool"))
        setBool(node.attribute("name").as_string(), node.attribute("value").as_bool());
      for (pugi::xml_node node = config.child("int"); node; node = node.next_sibling("int"))
        setInt(node.attribute("name").as_string(), node.attribute("value").as_int());
      for (pugi::xml_node node = config.child("float"); node; node = node.next_sibling("float"))
        setFloat(node.attribute("name").as_string(), node.attribute("value").as_float());
      for (pugi::xml_node node = config.child("string"); node; node = node.next_sibling("string"))
        setString(node.attribute("name").as_string(), node.attribute("value").as_string());
    } else { /* the old format, without the root config node -- keep for a transparent upgrade */
      for (pugi::xml_node node = doc.child("bool"); node; node = node.next_sibling("bool"))
        setBool(node.attribute("name").as_string(), node.attribute("value").as_bool());
      for (pugi::xml_node node = doc.child("int"); node; node = node.next_sibling("int"))
        setInt(node.attribute("name").as_string(), node.attribute("value").as_int());
      for (pugi::xml_node node = doc.child("float"); node; node = node.next_sibling("float"))
        setFloat(node.attribute("name").as_string(), node.attribute("value").as_float());
      for (pugi::xml_node node = doc.child("string"); node; node = node.next_sibling("string"))
        setString(node.attribute("name").as_string(), node.attribute("value").as_string());
    }

    return true;
}

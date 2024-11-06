#ifndef _CONFIG_H
#define _CONFIG_H
#include <string>

class ConfigLoader {
std::string FILE;
public:
    ConfigLoader(std::string);
    ~ConfigLoader();

    int getInt(std::string key, int defaultValue = -1);
    float getFloat(std::string key, float defaultValue = -1);
    void setFloat(std::string key, float value);
    std::string getString(std::string key, const std::string& defaultValue = "");
    bool getBool(std::string key, bool defaultValue = false);


};

#endif

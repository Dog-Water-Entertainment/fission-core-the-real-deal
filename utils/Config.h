#ifndef _CONFIG_H
#define _CONFIG_H
#include <string>

class ConfigLoader {
std::string FILE;
public:
    ConfigLoader(std::string);
    ~ConfigLoader();

    int getInt(std::string key);
    float getFloat(std::string key);
    std::string getString(std::string key);
    bool getBool(std::string key);


};

#endif

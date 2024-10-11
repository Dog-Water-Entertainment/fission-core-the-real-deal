#ifndef _CONFIG_H
#define _CONFIG_H

class ConfigLoader {
public:
    ConfigLoader();
    ~ConfigLoader();

    void loadConfig(const char* path);
    void saveConfig(const char* path);

    void set(const char* key, const char* value);
    const char* get(const char* key);

    void setInt(const char* key, int value);
    int getInt(const char* key);

    void setFloat(const char* key, float value);
    float getFloat(const char* key);

    void setBool(const char* key, bool value);
    bool getBool(const char* key);

    void setString(const char* key, const char* value);
    const char* getString(const char* key);

    void setDouble(const char* key, double value);
    double getDouble(const char* key);

    void setLong(const char* key, long value);
    long getLong(const char* key);

    void setLongLong(const char* key, long long value);
    long long getLongLong(const char* key);

    void setShort(const char* key, short value);
    short getShort(const char* key);

    void setChar(const char* key, char value);
    char getChar(const char* key);

    void setUChar(const char* key, unsigned char value);
    unsigned char getUChar(const char* key);

    void setUInt(const char* key, unsigned int value);
    unsigned int getUInt(const char* key);

    void setUShort(const char* key, unsigned short value);
    unsigned short getUShort(const char* key);

    void setULong(const char* key, unsigned long value);
    unsigned long getULong(const char* key);

    void setULongLong(const char* key, unsigned long long value);
    unsigned long long getULongLong(const char* key);

    void setFloatArray(const char* key, float* value, int size);
    float* getFloatArray(const char* key, int* size);

    void setIntArray(const char* key, int* value, int size);
    int* getIntArray(const char* key, int* size);

    void setBoolArray(const char* key, bool* value, int size);
    bool* getBoolArray(const char* key, int* size);

    void setStringArray(const char* key, const char** value, int size);
    const char** getStringArray(const char* key, int*
};

#endif
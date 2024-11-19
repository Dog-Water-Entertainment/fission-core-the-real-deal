// Elias Farmer

#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <chrono>

class TimeUtils
{
public:
    static double m_time_started;
    static double get_time();

private:
    static double initialize_time_started();
};

#endif

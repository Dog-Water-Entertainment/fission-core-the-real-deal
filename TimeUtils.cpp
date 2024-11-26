// Elias Farmer

#include "TimeUtils.h"
#include <chrono>
#include <iostream>

double TimeUtils::m_time_started = TimeUtils::initialize_time_started();

double TimeUtils::initialize_time_started()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto time_since_epoch = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(time_since_epoch).count();
}

double TimeUtils::get_time()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto time_since_epoch = now.time_since_epoch();
    double current_time = std::chrono::duration_cast<std::chrono::milliseconds>(time_since_epoch).count();
    return current_time - TimeUtils::m_time_started;
}


#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <atomic>

inline std::string GetTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto tt  = std::chrono::system_clock::to_time_t(now);
    std::tm tm{}; localtime_s(&tm, &tt);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

inline int GenerateId() {
    static std::atomic<int> counter{ 0 };
    return ++counter;
}

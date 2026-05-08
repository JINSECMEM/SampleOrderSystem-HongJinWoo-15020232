#pragma once
#include <string>

struct Sample {
    int         id                  = 0;
    std::string name;
    int         avg_production_time = 0;   // 분
    double      yield_rate          = 0.0; // 0.0 ~ 1.0
};

#pragma once
#include <string>
#include <vector>
#include <optional>
#include "../Models/Sample.h"

class SampleView {
public:
    void PrintList(const std::vector<Sample>& samples);
    void PrintDetail(const Sample& s, int stock);
    void PrintSearchResult(const std::vector<Sample>& samples);

    std::string GetNameInput();
    int         GetAvgTimeInput();
    double      GetYieldRateInput();
    int         GetIdInput();
    std::string GetKeywordInput();
};

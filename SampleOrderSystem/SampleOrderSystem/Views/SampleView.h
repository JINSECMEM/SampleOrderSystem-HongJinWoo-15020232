#pragma once
#include <string>
#include <vector>
#include <optional>
#include "../Models/Sample.h"

class SampleView {
public:
    void PrintList(const std::vector<Sample>& samples);
    void DisplayList(const std::vector<Sample>& samples);   // Pause 없이 표시
    void PrintDetail(const Sample& s, int stock);
    void PrintSearchResult(const std::vector<Sample>& samples);

    std::string GetNameInput();
    int         GetAvgTimeInput();
    double      GetYieldRateInput();
    int         GetIdInput();
    std::string GetKeywordInput();
    int         GetMaxTimeInput();
    double      GetMinYieldInput();
};

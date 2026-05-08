#include "SampleService.h"
#include <algorithm>

SampleService::SampleService(ISampleRepository& repo) : repo_(repo) {}

std::optional<Sample> SampleService::Register(const std::string& name, int avgTime, double yieldRate) {
    for (const auto& s : repo_.FindAll())
        if (s.name == name) return std::nullopt;
    Sample s;
    s.name                  = name;
    s.avg_production_time   = avgTime;
    s.yield_rate            = yieldRate;
    return repo_.Save(s);
}

std::optional<Sample> SampleService::FindById(int id) {
    return repo_.FindById(id);
}

std::vector<Sample> SampleService::FindAll() {
    return repo_.FindAll();
}

std::vector<Sample> SampleService::SearchByName(const std::string& keyword) {
    std::vector<Sample> result;
    for (const auto& s : repo_.FindAll())
        if (s.name.find(keyword) != std::string::npos)
            result.push_back(s);
    return result;
}

std::vector<Sample> SampleService::SearchByMaxTime(int maxTime) {
    std::vector<Sample> result;
    for (const auto& s : repo_.FindAll())
        if (s.avg_production_time <= maxTime)
            result.push_back(s);
    return result;
}

std::vector<Sample> SampleService::SearchByMinYield(double minYield) {
    std::vector<Sample> result;
    for (const auto& s : repo_.FindAll())
        if (s.yield_rate >= minYield)
            result.push_back(s);
    return result;
}

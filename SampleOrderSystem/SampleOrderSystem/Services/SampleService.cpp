#include "SampleService.h"
#include <algorithm>

SampleService::SampleService(ISampleRepository& repo) : repo_(repo) {}

Sample SampleService::Register(const std::string& name, int avgTime, double yieldRate) {
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
    for (const auto& s : repo_.FindAll()) {
        if (s.name.find(keyword) != std::string::npos)
            result.push_back(s);
    }
    return result;
}

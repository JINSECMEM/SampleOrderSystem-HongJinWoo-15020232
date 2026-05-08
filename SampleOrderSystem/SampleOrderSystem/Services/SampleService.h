#pragma once
#include <vector>
#include <optional>
#include <string>
#include "../Database/ISampleRepository.h"

class SampleService {
public:
    explicit SampleService(ISampleRepository& repo);
    Sample                    Register(const std::string& name, int avgTime, double yieldRate);
    std::optional<Sample>     FindById(int id);
    std::vector<Sample>       FindAll();
    std::vector<Sample>       SearchByName(const std::string& keyword);
private:
    ISampleRepository& repo_;
};

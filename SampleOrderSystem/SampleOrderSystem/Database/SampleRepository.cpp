#include "SampleRepository.h"

SampleRepository::SampleRepository() {
    std::vector<Sample> init = {
        {0, "Si-Wafer",  30, 0.95},
        {0, "SiC-Wafer", 60, 0.85},
        {0, "GaAs-Chip", 45, 0.80},
        {0, "GaN-Chip",  90, 0.75},
        {0, "InP-Chip",  120, 0.70},
    };
    for (auto& s : init) Save(s);
}

Sample SampleRepository::Save(const Sample& s) {
    Sample copy = s;
    copy.id = nextId_++;
    store_[copy.id] = copy;
    return copy;
}

std::optional<Sample> SampleRepository::FindById(int id) {
    auto it = store_.find(id);
    if (it == store_.end()) return std::nullopt;
    return it->second;
}

std::vector<Sample> SampleRepository::FindAll() {
    std::vector<Sample> result;
    for (const auto& [id, s] : store_) result.push_back(s);
    return result;
}

void SampleRepository::Update(const Sample& s) {
    store_[s.id] = s;
}

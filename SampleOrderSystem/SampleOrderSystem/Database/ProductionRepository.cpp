#include "ProductionRepository.h"
#include <stdexcept>

ProductionJob ProductionRepository::SaveJob(const ProductionJob& j) {
    ProductionJob copy = j;
    copy.id = nextId_++;
    store_[copy.id] = copy;
    return copy;
}

ProductionJob ProductionRepository::FindJobById(int id) {
    auto it = store_.find(id);
    if (it == store_.end()) throw std::runtime_error("ProductionJob not found: " + std::to_string(id));
    return it->second;
}

std::vector<ProductionJob> ProductionRepository::FindAllJobs() {
    std::vector<ProductionJob> result;
    for (const auto& [id, j] : store_) result.push_back(j);
    return result;
}

void ProductionRepository::UpdateJob(const ProductionJob& j) {
    store_[j.id] = j;
}

std::optional<ProductionJob> ProductionRepository::FindNextQueued() {
    for (const auto& [id, j] : store_)
        if (j.status == JobStatus::QUEUED) return j;
    return std::nullopt;
}

std::optional<ProductionJob> ProductionRepository::FindRunningJob() {
    for (const auto& [id, j] : store_)
        if (j.status == JobStatus::RUNNING) return j;
    return std::nullopt;
}

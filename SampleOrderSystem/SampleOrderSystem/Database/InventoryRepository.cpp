#include "InventoryRepository.h"
#include "../Utils/Utils.h"

InventoryRepository::InventoryRepository() {
    std::vector<Inventory> init = {
        {1, 50, GetTimestamp()},
        {2,  0, GetTimestamp()},
        {3, 20, GetTimestamp()},
        {4,  0, GetTimestamp()},
        {5, 10, GetTimestamp()},
    };
    for (auto& inv : init) Save(inv);
}

void InventoryRepository::Save(const Inventory& inv) {
    store_[inv.sample_id] = inv;
}

std::optional<Inventory> InventoryRepository::FindBySampleId(int sampleId) {
    auto it = store_.find(sampleId);
    if (it == store_.end()) return std::nullopt;
    return it->second;
}

std::vector<Inventory> InventoryRepository::FindAll() {
    std::vector<Inventory> result;
    for (const auto& [id, inv] : store_) result.push_back(inv);
    return result;
}

void InventoryRepository::Update(const Inventory& inv) {
    store_[inv.sample_id] = inv;
}

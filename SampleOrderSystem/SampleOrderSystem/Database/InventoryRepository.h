#pragma once
#include "IInventoryRepository.h"
#include <map>

class InventoryRepository : public IInventoryRepository {
public:
    InventoryRepository();
    void                     Save(const Inventory& inv)   override;
    std::optional<Inventory> FindBySampleId(int sampleId) override;
    std::vector<Inventory>   FindAll()                    override;
    void                     Update(const Inventory& inv) override;
private:
    std::map<int, Inventory> store_; // key = sample_id
};

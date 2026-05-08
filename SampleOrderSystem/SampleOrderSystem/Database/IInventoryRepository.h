#pragma once
#include <vector>
#include <optional>
#include "../Models/Inventory.h"

class IInventoryRepository {
public:
    virtual ~IInventoryRepository() = default;
    virtual void                       Save(const Inventory& inv)          = 0;
    virtual std::optional<Inventory>   FindBySampleId(int sampleId)        = 0;
    virtual std::vector<Inventory>     FindAll()                           = 0;
    virtual void                       Update(const Inventory& inv)        = 0;
};

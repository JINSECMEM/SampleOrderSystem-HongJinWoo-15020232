#pragma once
#include <vector>
#include <optional>
#include "../Models/ProductionLine.h"

class IProductionLineRepository {
public:
    virtual ~IProductionLineRepository() = default;
    virtual ProductionLine                  Save(const ProductionLine& l)   = 0;
    virtual std::optional<ProductionLine>   FindById(int id)                = 0;
    virtual std::vector<ProductionLine>     FindAll()                       = 0;
    virtual void                            Delete(int id)                  = 0;
    virtual void                            SaveToFile()  const             = 0;
};

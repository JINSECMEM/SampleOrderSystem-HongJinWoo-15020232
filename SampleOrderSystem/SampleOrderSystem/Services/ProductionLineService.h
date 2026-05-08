#pragma once
#include <vector>
#include <optional>
#include <string>
#include "../Database/IProductionLineRepository.h"
#include "../Database/IProductionRepository.h"

class ProductionLineService {
public:
    ProductionLineService(IProductionLineRepository& lineRepo, IProductionRepository& jobRepo);
    ProductionLine                  AddLine(const std::string& name);
    bool                            DeleteLine(int lineId);   // false = RUNNING job 있음
    std::vector<ProductionLine>     GetAllLines();
    std::optional<ProductionLine>   FindById(int id);
private:
    IProductionLineRepository& lineRepo_;
    IProductionRepository&     jobRepo_;
};

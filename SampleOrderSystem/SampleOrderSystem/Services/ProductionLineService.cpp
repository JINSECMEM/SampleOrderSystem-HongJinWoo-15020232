#include "ProductionLineService.h"
#include "../Utils/Utils.h"

ProductionLineService::ProductionLineService(IProductionLineRepository& lineRepo,
                                              IProductionRepository&     jobRepo)
    : lineRepo_(lineRepo), jobRepo_(jobRepo) {}

ProductionLine ProductionLineService::AddLine(const std::string& name) {
    ProductionLine l;
    l.name       = name;
    l.created_at = GetTimestamp();
    return lineRepo_.Save(l);
}

bool ProductionLineService::DeleteLine(int lineId) {
    if (jobRepo_.HasRunningJobOnLine(lineId)) return false;
    lineRepo_.Delete(lineId);
    return true;
}

std::vector<ProductionLine> ProductionLineService::GetAllLines() {
    return lineRepo_.FindAll();
}

std::optional<ProductionLine> ProductionLineService::FindById(int id) {
    return lineRepo_.FindById(id);
}

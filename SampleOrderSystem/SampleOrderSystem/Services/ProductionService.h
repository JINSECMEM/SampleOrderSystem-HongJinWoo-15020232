#pragma once
#include <vector>
#include <optional>
#include <functional>
#include "../Database/IProductionRepository.h"
#include "../Database/IProductionLineRepository.h"

class ProductionService {
public:
    using OnCompleteCallback = std::function<void(const ProductionJob&)>;

    ProductionService(IProductionRepository& repo, IProductionLineRepository& lineRepo);
    void SetOnCompleteCallback(OnCompleteCallback cb);

    ProductionJob              Enqueue(int orderId, int sampleId,
                                       int shortage, double yieldRate, int avgProdTime);
    void                       Tick();
    std::vector<ProductionJob> GetAllJobs();
    std::vector<ProductionJob> GetAllRunningJobs();
    std::vector<ProductionJob> GetQueuedJobs();

private:
    static constexpr double PRODUCTION_SAFETY_MARGIN = 0.9;
    void TickLine(int lineId);

    IProductionRepository&     repo_;
    IProductionLineRepository& lineRepo_;
    OnCompleteCallback         onComplete_;
};

#pragma once
#include <vector>
#include <optional>
#include <functional>
#include "../Database/IProductionRepository.h"

class ProductionService {
public:
    using OnCompleteCallback = std::function<void(const ProductionJob&)>;

    explicit ProductionService(IProductionRepository& repo);
    void SetOnCompleteCallback(OnCompleteCallback cb);

    ProductionJob              Enqueue(int orderId, int sampleId,
                                      int shortage, double yieldRate, int avgProdTime);
    void                       Tick();
    std::vector<ProductionJob> GetAllJobs();
    std::optional<ProductionJob> GetRunningJob();
    std::vector<ProductionJob> GetQueuedJobs();

private:
    static constexpr int    TICK_MINUTES              = 10;
    static constexpr double PRODUCTION_SAFETY_MARGIN  = 0.9;

    IProductionRepository& repo_;
    OnCompleteCallback     onComplete_;
};

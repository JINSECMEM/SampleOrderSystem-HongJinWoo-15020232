#include "ProductionService.h"
#include "../Utils/Utils.h"
#include <cmath>

ProductionService::ProductionService(IProductionRepository& repo) : repo_(repo) {}

void ProductionService::SetOnCompleteCallback(OnCompleteCallback cb) {
    onComplete_ = std::move(cb);
}

ProductionJob ProductionService::Enqueue(int orderId, int sampleId,
                                         int shortage, double yieldRate, int avgProdTime) {
    int actualQty   = static_cast<int>(std::ceil(shortage / (yieldRate * PRODUCTION_SAFETY_MARGIN)));
    int totalTime   = avgProdTime * actualQty;

    ProductionJob j;
    j.order_id       = orderId;
    j.sample_id      = sampleId;
    j.target_qty     = actualQty;
    j.total_time_min = totalTime;
    j.status         = JobStatus::QUEUED;
    j.created_at     = GetTimestamp();
    return repo_.SaveJob(j);
}

void ProductionService::Tick() {
    auto running = repo_.FindRunningJob();
    if (!running) {
        auto next = repo_.FindNextQueued();
        if (!next) return;
        next->status     = JobStatus::RUNNING;
        next->start_time = std::time(nullptr);
        repo_.UpdateJob(*next);
        running = next;
    }

    // 실제 경과 시간(초) = 시뮬레이션 분 (1초 = 1분)
    running->elapsed_min = static_cast<int>(std::time(nullptr) - running->start_time);

    if (running->elapsed_min >= running->total_time_min) {
        running->produced_qty = running->target_qty;
        running->status       = JobStatus::COMPLETED;
        running->completed_at = GetTimestamp();
        repo_.UpdateJob(*running);
        if (onComplete_) onComplete_(*running);
    } else {
        repo_.UpdateJob(*running);
    }
}

std::vector<ProductionJob> ProductionService::GetAllJobs() {
    return repo_.FindAllJobs();
}

std::optional<ProductionJob> ProductionService::GetRunningJob() {
    return repo_.FindRunningJob();
}

std::vector<ProductionJob> ProductionService::GetQueuedJobs() {
    std::vector<ProductionJob> result;
    for (const auto& j : repo_.FindAllJobs())
        if (j.status == JobStatus::QUEUED) result.push_back(j);
    return result;
}

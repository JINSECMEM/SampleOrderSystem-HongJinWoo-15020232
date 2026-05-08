#include "ProductionService.h"
#include "../Utils/Utils.h"
#include <cmath>
#include <ctime>

ProductionService::ProductionService(IProductionRepository& repo,
                                      IProductionLineRepository& lineRepo)
    : repo_(repo), lineRepo_(lineRepo) {}

void ProductionService::SetOnCompleteCallback(OnCompleteCallback cb) {
    onComplete_ = std::move(cb);
}

ProductionJob ProductionService::Enqueue(int orderId, int sampleId,
                                          int shortage, double yieldRate, int avgProdTime) {
    int actualQty  = static_cast<int>(std::ceil(shortage / (yieldRate * PRODUCTION_SAFETY_MARGIN)));
    int totalTime  = avgProdTime * actualQty;

    ProductionJob j;
    j.order_id       = orderId;
    j.sample_id      = sampleId;
    j.line_id        = 0;   // 미배정, Tick에서 라인 배정
    j.target_qty     = actualQty;
    j.total_time_min = totalTime;
    j.status         = JobStatus::QUEUED;
    j.created_at     = GetTimestamp();
    return repo_.SaveJob(j);
}

void ProductionService::Tick() {
    auto lines = lineRepo_.FindAll();
    if (lines.empty()) {
        TickLine(0);
    } else {
        for (const auto& line : lines) TickLine(line.id);
    }
    repo_.SaveToFile();
}

void ProductionService::TickLine(int lineId) {
    auto running = repo_.FindRunningJobByLine(lineId);
    if (!running) {
        auto next = repo_.FindNextQueued();   // line_id==0 QUEUED
        if (!next) return;
        next->line_id    = lineId;
        next->status     = JobStatus::RUNNING;
        next->start_time = std::time(nullptr);
        repo_.UpdateJob(*next);
        running = next;
    }

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

std::vector<ProductionJob> ProductionService::GetAllRunningJobs() {
    return repo_.FindAllRunningJobs();
}

std::vector<ProductionJob> ProductionService::GetQueuedJobs() {
    std::vector<ProductionJob> result;
    for (const auto& j : repo_.FindAllJobs())
        if (j.status == JobStatus::QUEUED) result.push_back(j);
    return result;
}

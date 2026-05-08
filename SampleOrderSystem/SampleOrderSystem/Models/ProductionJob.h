#pragma once
#include <string>
#include <ctime>

enum class JobStatus { QUEUED, RUNNING, COMPLETED };

struct ProductionJob {
    int         id            = 0;
    int         order_id      = 0;
    int         sample_id     = 0;
    int         line_id       = 0;   // 0 = 미배정
    int         target_qty    = 0;
    int         produced_qty  = 0;
    int         total_time_min = 0;
    int         elapsed_min   = 0;
    std::time_t start_time    = 0;   // RUNNING 전환 시 기록 (실시간 경과 계산용)
    JobStatus   status        = JobStatus::QUEUED;
    std::string created_at;
    std::string completed_at;
};

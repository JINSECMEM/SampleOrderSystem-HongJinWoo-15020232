#pragma once
#include <string>

enum class JobStatus { QUEUED, RUNNING, COMPLETED };

struct ProductionJob {
    int       id            = 0;
    int       order_id      = 0;
    int       sample_id     = 0;
    int       target_qty    = 0;
    int       produced_qty  = 0;
    int       total_time_min = 0;
    int       elapsed_min   = 0;
    JobStatus status        = JobStatus::QUEUED;
    std::string created_at;
    std::string completed_at;
};

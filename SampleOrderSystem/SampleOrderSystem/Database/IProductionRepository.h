#pragma once
#include <vector>
#include <optional>
#include "../Models/ProductionJob.h"

class IProductionRepository {
public:
    virtual ~IProductionRepository() = default;
    virtual ProductionJob                  SaveJob(const ProductionJob& j)           = 0;
    virtual ProductionJob                  FindJobById(int id)                       = 0;
    virtual std::vector<ProductionJob>     FindAllJobs()                             = 0;
    virtual void                           UpdateJob(const ProductionJob& j)         = 0;
    virtual std::optional<ProductionJob>   FindNextQueued()                          = 0; // line_id==0 FIFO
    virtual std::optional<ProductionJob>   FindRunningJobByLine(int lineId)          = 0;
    virtual std::vector<ProductionJob>     FindAllRunningJobs()                      = 0;
    virtual bool                           HasRunningJobOnLine(int lineId)   const   = 0;
    virtual void                           SaveToFile()   const                      = 0;
};

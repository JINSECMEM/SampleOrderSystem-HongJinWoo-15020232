#pragma once
#include "IProductionRepository.h"
#include <map>

class ProductionRepository : public IProductionRepository {
public:
    ProductionRepository();
    ProductionJob                  SaveJob(const ProductionJob& j)          override;
    ProductionJob                  FindJobById(int id)                      override;
    std::vector<ProductionJob>     FindAllJobs()                            override;
    void                           UpdateJob(const ProductionJob& j)        override;
    std::optional<ProductionJob>   FindNextQueued()                         override;
    std::optional<ProductionJob>   FindRunningJobByLine(int lineId)         override;
    std::vector<ProductionJob>     FindAllRunningJobs()                     override;
    bool                           HasRunningJobOnLine(int lineId)  const   override;
    void                           SaveToFile()   const                     override;
private:
    static constexpr const char* DATA_FILE = "data/production_jobs.dat";
    void LoadFromFile();
    std::map<int, ProductionJob> store_;
    int nextId_ = 1;
};

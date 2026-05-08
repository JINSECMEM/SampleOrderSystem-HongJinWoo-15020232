#pragma once
#include "IProductionRepository.h"
#include <map>

class ProductionRepository : public IProductionRepository {
public:
    ProductionJob                SaveJob(const ProductionJob& j)   override;
    ProductionJob                FindJobById(int id)               override;
    std::vector<ProductionJob>   FindAllJobs()                     override;
    void                         UpdateJob(const ProductionJob& j)  override;
    std::optional<ProductionJob> FindNextQueued()                  override;
    std::optional<ProductionJob> FindRunningJob()                  override;
private:
    std::map<int, ProductionJob> store_;
    int nextId_ = 1;
};

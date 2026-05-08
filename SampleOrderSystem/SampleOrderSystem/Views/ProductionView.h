#pragma once
#include <vector>
#include "../Models/ProductionJob.h"
#include "../Models/ProductionLine.h"
#include "../Models/Sample.h"
#include "../Models/Order.h"

class ProductionView {
public:
    void Render(const std::vector<ProductionLine>& lines,
                const std::vector<ProductionJob>&  runningJobs,
                const std::vector<ProductionJob>&  queued,
                const std::vector<Sample>&         samples,
                const std::vector<Order>&          orders);
};

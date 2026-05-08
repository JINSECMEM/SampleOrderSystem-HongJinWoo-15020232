#pragma once
#include <vector>
#include <optional>
#include "../Models/ProductionJob.h"
#include "../Models/Sample.h"

class ProductionView {
public:
    void Render(const std::optional<ProductionJob>& running,
                const std::vector<ProductionJob>& queued,
                const std::vector<Sample>& samples);
};

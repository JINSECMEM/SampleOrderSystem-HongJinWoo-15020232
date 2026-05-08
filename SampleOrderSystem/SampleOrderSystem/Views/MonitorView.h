#pragma once
#include <vector>
#include <string>
#include <optional>
#include "../Models/Order.h"
#include "../Models/Inventory.h"
#include "../Models/Sample.h"
#include "../Models/ProductionJob.h"
#include "../Services/InventoryService.h"

class MonitorView {
public:
    void Render(const std::vector<Order>& orders,
                const std::vector<Inventory>& inventories,
                const std::vector<Sample>& samples,
                const std::vector<ProductionJob>& runningJobs,
                const std::vector<ProductionJob>& queued);
};

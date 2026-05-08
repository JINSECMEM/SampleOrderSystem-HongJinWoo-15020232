#pragma once
#include <vector>
#include "../Database/IOrderRepository.h"
#include "InventoryService.h"

class ReleaseService {
public:
    ReleaseService(IOrderRepository& ordRepo, InventoryService& invSvc);
    bool               Release(int orderId);
    std::vector<Order> GetConfirmedOrders();
private:
    IOrderRepository& ordRepo_;
    InventoryService& invSvc_;
};

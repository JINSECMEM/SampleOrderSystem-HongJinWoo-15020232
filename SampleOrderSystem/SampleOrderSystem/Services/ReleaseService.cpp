#include "ReleaseService.h"
#include "../Utils/Utils.h"

ReleaseService::ReleaseService(IOrderRepository& ordRepo, InventoryService& invSvc)
    : ordRepo_(ordRepo), invSvc_(invSvc) {}

bool ReleaseService::Release(int orderId) {
    Order o = ordRepo_.FindById(orderId);
    if (o.status != OrderStatus::CONFIRMED) return false;
    invSvc_.Deduct(o.sample_id, o.quantity);
    o.status     = OrderStatus::RELEASE;
    o.updated_at = GetTimestamp();
    ordRepo_.Update(o);
    return true;
}

std::vector<Order> ReleaseService::GetConfirmedOrders() {
    return ordRepo_.FindByStatus(OrderStatus::CONFIRMED);
}

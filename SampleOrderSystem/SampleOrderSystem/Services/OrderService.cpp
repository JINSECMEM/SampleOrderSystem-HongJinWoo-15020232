#include "OrderService.h"
#include "../Utils/Utils.h"

OrderService::OrderService(IOrderRepository& repo) : repo_(repo) {}

Order OrderService::PlaceOrder(int sampleId, int qty) {
    Order o;
    o.sample_id  = sampleId;
    o.quantity   = qty;
    o.status     = OrderStatus::RESERVED;
    o.created_at = GetTimestamp();
    o.updated_at = GetTimestamp();
    return repo_.Save(o);
}

void OrderService::Reject(int orderId) {
    UpdateStatus(orderId, OrderStatus::REJECTED);
}

void OrderService::UpdateStatus(int orderId, OrderStatus status) {
    Order o      = repo_.FindById(orderId);
    o.status     = status;
    o.updated_at = GetTimestamp();
    repo_.Update(o);
}

Order OrderService::FindById(int orderId) {
    return repo_.FindById(orderId);
}

std::vector<Order> OrderService::FindAll() {
    return repo_.FindAll();
}

std::vector<Order> OrderService::FindByStatus(OrderStatus status) {
    return repo_.FindByStatus(status);
}

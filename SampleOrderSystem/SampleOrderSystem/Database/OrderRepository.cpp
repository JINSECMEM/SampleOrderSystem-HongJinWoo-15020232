#include "OrderRepository.h"
#include <stdexcept>

Order OrderRepository::Save(const Order& o) {
    Order copy = o;
    copy.id = nextId_++;
    store_[copy.id] = copy;
    return copy;
}

Order OrderRepository::FindById(int id) {
    auto it = store_.find(id);
    if (it == store_.end()) throw std::runtime_error("Order not found: " + std::to_string(id));
    return it->second;
}

std::vector<Order> OrderRepository::FindAll() {
    std::vector<Order> result;
    for (const auto& [id, o] : store_) result.push_back(o);
    return result;
}

void OrderRepository::Update(const Order& o) {
    store_[o.id] = o;
}

std::vector<Order> OrderRepository::FindByStatus(OrderStatus status) {
    std::vector<Order> result;
    for (const auto& [id, o] : store_)
        if (o.status == status) result.push_back(o);
    return result;
}

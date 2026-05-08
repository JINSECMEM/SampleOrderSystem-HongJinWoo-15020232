#pragma once
#include "IOrderRepository.h"
#include <map>

class OrderRepository : public IOrderRepository {
public:
    Order              Save(const Order& o)             override;
    Order              FindById(int id)                 override;
    std::vector<Order> FindAll()                        override;
    void               Update(const Order& o)           override;
    std::vector<Order> FindByStatus(OrderStatus status) override;
private:
    std::map<int, Order> store_;
    int nextId_ = 1;
};

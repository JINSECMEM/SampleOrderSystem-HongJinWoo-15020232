#pragma once
#include <vector>
#include "../Database/IOrderRepository.h"

class OrderService {
public:
    explicit OrderService(IOrderRepository& repo);
    Order               PlaceOrder(int sampleId, int qty);
    void                Reject(int orderId);
    void                UpdateStatus(int orderId, OrderStatus status);
    Order               FindById(int orderId);
    std::vector<Order>  FindAll();
    std::vector<Order>  FindByStatus(OrderStatus status);
private:
    IOrderRepository& repo_;
};

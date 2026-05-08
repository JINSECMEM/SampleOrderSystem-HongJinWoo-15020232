#pragma once
#include <vector>
#include "../Models/Order.h"

class IOrderRepository {
public:
    virtual ~IOrderRepository() = default;
    virtual Order               Save(const Order& o)              = 0;
    virtual Order               FindById(int id)                  = 0;
    virtual std::vector<Order>  FindAll()                         = 0;
    virtual void                Update(const Order& o)            = 0;
    virtual std::vector<Order>  FindByStatus(OrderStatus status)  = 0;
    virtual void                SaveToFile()  const               = 0;
};

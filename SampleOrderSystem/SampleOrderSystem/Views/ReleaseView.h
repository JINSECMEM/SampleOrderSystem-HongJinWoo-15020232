#pragma once
#include <vector>
#include "../Models/Order.h"
#include "../Models/Sample.h"

class ReleaseView {
public:
    void PrintConfirmedOrders(const std::vector<Order>& orders, const std::vector<Sample>& samples);
    int  GetOrderIdInput();
};

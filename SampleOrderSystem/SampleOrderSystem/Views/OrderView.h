#pragma once
#include <vector>
#include "../Models/Order.h"
#include "../Models/Sample.h"

struct OrderInput {
    int sample_id = 0;
    int quantity  = 0;
};

struct ApprovalInput {
    int  order_id = 0;
    bool approve  = false;
};

class OrderView {
public:
    void          PrintList(const std::vector<Order>& orders, const std::vector<Sample>& samples);
    OrderInput    GetOrderInput(const std::vector<Sample>& samples);
    ApprovalInput GetApprovalInput(const std::vector<Order>& reserved, const std::vector<Sample>& samples);
};

#pragma once
#include <string>

enum class OrderStatus { RESERVED, REJECTED, CONFIRMED, PRODUCING, RELEASE };

inline const char* OrderStatusStr(OrderStatus s) {
    switch (s) {
    case OrderStatus::RESERVED:  return "RESERVED";
    case OrderStatus::REJECTED:  return "REJECTED";
    case OrderStatus::CONFIRMED: return "CONFIRMED";
    case OrderStatus::PRODUCING: return "PRODUCING";
    case OrderStatus::RELEASE:   return "RELEASE";
    default:                     return "UNKNOWN";
    }
}

struct Order {
    int         id        = 0;
    int         sample_id = 0;
    int         quantity  = 0;
    OrderStatus status    = OrderStatus::RESERVED;
    std::string created_at;
    std::string updated_at;
};

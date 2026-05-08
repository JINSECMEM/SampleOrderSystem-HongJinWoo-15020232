#pragma once
#include <gmock/gmock.h>
#include "Database/IOrderRepository.h"

class MockOrderRepository : public IOrderRepository {
public:
    MOCK_METHOD(Order,              Save,         (const Order&),      (override));
    MOCK_METHOD(Order,              FindById,     (int),               (override));
    MOCK_METHOD(std::vector<Order>, FindAll,      (),                  (override));
    MOCK_METHOD(void,               Update,       (const Order&),      (override));
    MOCK_METHOD(std::vector<Order>, FindByStatus, (OrderStatus),       (override));
    MOCK_METHOD(void,               SaveToFile,   (),                  (const, override));
};

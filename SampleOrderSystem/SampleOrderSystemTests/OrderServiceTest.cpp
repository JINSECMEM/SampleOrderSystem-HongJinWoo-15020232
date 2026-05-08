#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Mocks/MockOrderRepository.h"
#include "Services/OrderService.h"

using ::testing::_;
using ::testing::Return;
using ::testing::Field;

TEST(OrderServiceTest, PlaceOrder_ReturnsSavedOrderWithReservedStatus) {
    MockOrderRepository repo;
    Order saved;
    saved.id = 1; saved.sample_id = 2; saved.quantity = 10;
    saved.status = OrderStatus::RESERVED;
    EXPECT_CALL(repo, Save(_)).WillOnce(Return(saved));

    OrderService svc(repo);
    Order result = svc.PlaceOrder(2, 10, "TestCo");
    EXPECT_EQ(result.id, 1);
    EXPECT_EQ(result.status, OrderStatus::RESERVED);
}

TEST(OrderServiceTest, Reject_SetsStatusToRejected) {
    MockOrderRepository repo;
    Order order; order.id = 1; order.status = OrderStatus::RESERVED;
    EXPECT_CALL(repo, FindById(1)).WillOnce(Return(order));
    EXPECT_CALL(repo, Update(Field(&Order::status, OrderStatus::REJECTED)));

    OrderService svc(repo);
    svc.Reject(1);
}

TEST(OrderServiceTest, UpdateStatus_ToConfirmed) {
    MockOrderRepository repo;
    Order order; order.id = 1; order.status = OrderStatus::RESERVED;
    EXPECT_CALL(repo, FindById(1)).WillOnce(Return(order));
    EXPECT_CALL(repo, Update(Field(&Order::status, OrderStatus::CONFIRMED)));

    OrderService svc(repo);
    svc.UpdateStatus(1, OrderStatus::CONFIRMED);
}

TEST(OrderServiceTest, FindByStatus_DelegatesToRepo) {
    MockOrderRepository repo;
    std::vector<Order> reserved = { {1, 1, 5, "", OrderStatus::RESERVED} };
    EXPECT_CALL(repo, FindByStatus(OrderStatus::RESERVED)).WillOnce(Return(reserved));

    OrderService svc(repo);
    auto result = svc.FindByStatus(OrderStatus::RESERVED);
    EXPECT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].id, 1);
}

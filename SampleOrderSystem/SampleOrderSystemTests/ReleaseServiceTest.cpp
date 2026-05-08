#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Mocks/MockOrderRepository.h"
#include "Mocks/MockInventoryRepository.h"
#include "Services/InventoryService.h"
#include "Services/ReleaseService.h"

using ::testing::_;
using ::testing::Return;
using ::testing::Field;

TEST(ReleaseServiceTest, Release_ConfirmedOrder_ReturnsTrue) {
    MockOrderRepository ordRepo;
    MockInventoryRepository invRepo;
    InventoryService invSvc(invRepo);

    Order order; order.id = 1; order.sample_id = 2;
    order.quantity = 5; order.status = OrderStatus::CONFIRMED;
    Inventory inv{2, 50};

    EXPECT_CALL(ordRepo, FindById(1)).WillOnce(Return(order));
    EXPECT_CALL(invRepo, FindBySampleId(2)).WillRepeatedly(Return(inv));
    EXPECT_CALL(invRepo, Update(_)).Times(1);
    EXPECT_CALL(ordRepo, Update(Field(&Order::status, OrderStatus::RELEASE)));

    ReleaseService svc(ordRepo, invSvc);
    EXPECT_TRUE(svc.Release(1));
}

TEST(ReleaseServiceTest, Release_NonConfirmedOrder_ReturnsFalse) {
    MockOrderRepository ordRepo;
    MockInventoryRepository invRepo;
    InventoryService invSvc(invRepo);

    Order order; order.id = 1; order.status = OrderStatus::RESERVED;
    EXPECT_CALL(ordRepo, FindById(1)).WillOnce(Return(order));

    ReleaseService svc(ordRepo, invSvc);
    EXPECT_FALSE(svc.Release(1));
}

TEST(ReleaseServiceTest, GetConfirmedOrders_DelegatesToRepo) {
    MockOrderRepository ordRepo;
    MockInventoryRepository invRepo;
    InventoryService invSvc(invRepo);

    std::vector<Order> confirmed = { {1, 1, 10, OrderStatus::CONFIRMED} };
    EXPECT_CALL(ordRepo, FindByStatus(OrderStatus::CONFIRMED)).WillOnce(Return(confirmed));

    ReleaseService svc(ordRepo, invSvc);
    auto result = svc.GetConfirmedOrders();
    EXPECT_EQ(result.size(), 1u);
}

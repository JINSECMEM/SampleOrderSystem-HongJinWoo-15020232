#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Mocks/MockInventoryRepository.h"
#include "Services/InventoryService.h"

using ::testing::_;
using ::testing::Return;

TEST(InventoryServiceTest, CheckStock_Sufficient_ReturnsTrue) {
    MockInventoryRepository repo;
    EXPECT_CALL(repo, FindBySampleId(1)).WillOnce(Return(Inventory{1, 100}));
    InventoryService svc(repo);
    EXPECT_TRUE(svc.CheckStock(1, 50));
}

TEST(InventoryServiceTest, CheckStock_Insufficient_ReturnsFalse) {
    MockInventoryRepository repo;
    EXPECT_CALL(repo, FindBySampleId(1)).WillOnce(Return(Inventory{1, 30}));
    InventoryService svc(repo);
    EXPECT_FALSE(svc.CheckStock(1, 50));
}

TEST(InventoryServiceTest, CheckStock_Exact_ReturnsTrue) {
    MockInventoryRepository repo;
    EXPECT_CALL(repo, FindBySampleId(1)).WillOnce(Return(Inventory{1, 10}));
    InventoryService svc(repo);
    EXPECT_TRUE(svc.CheckStock(1, 10));
}

TEST(InventoryServiceTest, GetStockStatus_Depleted) {
    MockInventoryRepository repo;
    EXPECT_CALL(repo, FindBySampleId(1)).WillRepeatedly(Return(Inventory{1, 0}));
    InventoryService svc(repo);
    EXPECT_EQ(svc.GetStockStatus(1, 10), StockStatus::DEPLETED);
}

TEST(InventoryServiceTest, GetStockStatus_Short) {
    MockInventoryRepository repo;
    EXPECT_CALL(repo, FindBySampleId(1)).WillRepeatedly(Return(Inventory{1, 5}));
    InventoryService svc(repo);
    EXPECT_EQ(svc.GetStockStatus(1, 10), StockStatus::SHORT);
}

TEST(InventoryServiceTest, GetStockStatus_Sufficient) {
    MockInventoryRepository repo;
    EXPECT_CALL(repo, FindBySampleId(1)).WillRepeatedly(Return(Inventory{1, 10}));
    InventoryService svc(repo);
    EXPECT_EQ(svc.GetStockStatus(1, 10), StockStatus::SUFFICIENT);
}

TEST(InventoryServiceTest, Replenish_IncreasesQuantity) {
    MockInventoryRepository repo;
    Inventory inv{1, 10};
    EXPECT_CALL(repo, FindBySampleId(1)).WillOnce(Return(inv));
    EXPECT_CALL(repo, Update(testing::Field(&Inventory::quantity, 25)));
    InventoryService svc(repo);
    svc.Replenish(1, 15);
}

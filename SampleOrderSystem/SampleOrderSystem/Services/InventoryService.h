#pragma once
#include <vector>
#include "../Database/IInventoryRepository.h"

enum class StockStatus { SUFFICIENT, SHORT, DEPLETED };

class InventoryService {
public:
    explicit InventoryService(IInventoryRepository& repo);
    bool        CheckStock(int sampleId, int requiredQty);
    void        Deduct(int sampleId, int qty);
    void        Replenish(int sampleId, int qty);
    int         GetStock(int sampleId);
    StockStatus GetStockStatus(int sampleId, int requiredQty);
    std::vector<Inventory> GetAll();
private:
    IInventoryRepository& repo_;
};

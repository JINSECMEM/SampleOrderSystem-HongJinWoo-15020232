#include "InventoryService.h"
#include "../Utils/Utils.h"
#include <stdexcept>

InventoryService::InventoryService(IInventoryRepository& repo) : repo_(repo) {}

bool InventoryService::CheckStock(int sampleId, int requiredQty) {
    return GetStock(sampleId) >= requiredQty;
}

void InventoryService::Deduct(int sampleId, int qty) {
    auto inv = repo_.FindBySampleId(sampleId);
    if (!inv) throw std::runtime_error("Inventory not found for sample: " + std::to_string(sampleId));
    if (inv->quantity < qty) throw std::runtime_error("Insufficient stock");
    inv->quantity     -= qty;
    inv->last_updated  = GetTimestamp();
    repo_.Update(*inv);
}

void InventoryService::Replenish(int sampleId, int qty) {
    auto inv = repo_.FindBySampleId(sampleId);
    if (!inv) {
        Inventory newInv{ sampleId, qty, GetTimestamp() };
        repo_.Save(newInv);
        return;
    }
    inv->quantity     += qty;
    inv->last_updated  = GetTimestamp();
    repo_.Update(*inv);
}

int InventoryService::GetStock(int sampleId) {
    auto inv = repo_.FindBySampleId(sampleId);
    return inv ? inv->quantity : 0;
}

StockStatus InventoryService::GetStockStatus(int sampleId, int requiredQty) {
    int qty = GetStock(sampleId);
    if (qty == 0)          return StockStatus::DEPLETED;
    if (qty < requiredQty) return StockStatus::SHORT;
    return StockStatus::SUFFICIENT;
}

std::vector<Inventory> InventoryService::GetAll() {
    return repo_.FindAll();
}

#pragma once
#include <functional>
#include <string>
#include "../Services/OrderService.h"
#include "../Services/SampleService.h"
#include "../Services/InventoryService.h"
#include "../Services/ProductionService.h"
#include "../Views/OrderView.h"

class OrderController {
public:
    using LogCallback = std::function<void(const std::string&)>;

    OrderController(OrderService& orderSvc, SampleService& sampleSvc,
                    InventoryService& invSvc, ProductionService& prodSvc,
                    OrderView& view, LogCallback log);
    void PlaceOrder();
    void ProcessPending();
private:
    OrderService&      orderSvc_;
    SampleService&     sampleSvc_;
    InventoryService&  invSvc_;
    ProductionService& prodSvc_;
    OrderView&         view_;
    LogCallback        log_;
};

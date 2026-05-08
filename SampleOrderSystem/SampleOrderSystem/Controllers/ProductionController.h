#pragma once
#include "../Services/ProductionService.h"
#include "../Services/SampleService.h"
#include "../Services/OrderService.h"
#include "../Views/ProductionView.h"

class ProductionController {
public:
    ProductionController(ProductionService& prodSvc, SampleService& sampleSvc,
                         OrderService& orderSvc, ProductionView& view);
    void ShowProduction();
private:
    ProductionService& prodSvc_;
    SampleService&     sampleSvc_;
    OrderService&      orderSvc_;
    ProductionView&    view_;
};

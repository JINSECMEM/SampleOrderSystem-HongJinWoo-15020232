#pragma once
#include "../Services/ProductionService.h"
#include "../Services/ProductionLineService.h"
#include "../Services/SampleService.h"
#include "../Services/OrderService.h"
#include "../Views/ProductionView.h"
#include "../Views/ProductionLineView.h"

class ProductionController {
public:
    ProductionController(ProductionService& prodSvc, ProductionLineService& lineSvc,
                         SampleService& sampleSvc, OrderService& orderSvc,
                         ProductionView& view, ProductionLineView& lineView);
    void ShowProduction();
private:
    void ShowLines();
    void AddLine();
    void DeleteLine();

    ProductionService&     prodSvc_;
    ProductionLineService& lineSvc_;
    SampleService&         sampleSvc_;
    OrderService&          orderSvc_;
    ProductionView&        view_;
    ProductionLineView&    lineView_;
};

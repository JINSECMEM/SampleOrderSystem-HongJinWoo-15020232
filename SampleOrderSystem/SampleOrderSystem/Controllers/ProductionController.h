#pragma once
#include "../Services/ProductionService.h"
#include "../Services/SampleService.h"
#include "../Views/ProductionView.h"

class ProductionController {
public:
    ProductionController(ProductionService& prodSvc, SampleService& sampleSvc, ProductionView& view);
    void ShowProduction();
private:
    ProductionService& prodSvc_;
    SampleService&     sampleSvc_;
    ProductionView&    view_;
};

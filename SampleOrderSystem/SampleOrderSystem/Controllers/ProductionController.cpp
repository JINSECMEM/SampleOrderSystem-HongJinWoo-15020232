#include "ProductionController.h"

ProductionController::ProductionController(ProductionService& prodSvc,
                                            SampleService& sampleSvc,
                                            OrderService& orderSvc,
                                            ProductionView& view)
    : prodSvc_(prodSvc), sampleSvc_(sampleSvc), orderSvc_(orderSvc), view_(view) {}

void ProductionController::ShowProduction() {
    view_.Render(prodSvc_.GetRunningJob(),
                 prodSvc_.GetQueuedJobs(),
                 sampleSvc_.FindAll(),
                 orderSvc_.FindAll());
}

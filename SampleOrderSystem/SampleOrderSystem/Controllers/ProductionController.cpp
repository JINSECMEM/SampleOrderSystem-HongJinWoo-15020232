#include "ProductionController.h"

ProductionController::ProductionController(ProductionService& prodSvc,
                                            SampleService& sampleSvc,
                                            ProductionView& view)
    : prodSvc_(prodSvc), sampleSvc_(sampleSvc), view_(view) {}

void ProductionController::ShowProduction() {
    view_.Render(prodSvc_.GetRunningJob(),
                 prodSvc_.GetQueuedJobs(),
                 sampleSvc_.FindAll());
}

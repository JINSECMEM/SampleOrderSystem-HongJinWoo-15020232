#pragma once
#include "../Services/SampleService.h"
#include "../Services/InventoryService.h"
#include "../Views/SampleView.h"

class SampleController {
public:
    SampleController(SampleService& sampleSvc, InventoryService& invSvc, SampleView& view);
    void RunMenu();
private:
    void Register();
    void ListAll();
    void Search();
    SampleService&   sampleSvc_;
    InventoryService& invSvc_;
    SampleView&      view_;
};

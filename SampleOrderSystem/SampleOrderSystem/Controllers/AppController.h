#pragma once
#include "../Database/SampleRepository.h"
#include "../Database/OrderRepository.h"
#include "../Database/InventoryRepository.h"
#include "../Database/ProductionRepository.h"
#include "../Database/ProductionLineRepository.h"
#include "../Services/SampleService.h"
#include "../Services/OrderService.h"
#include "../Services/InventoryService.h"
#include "../Services/ProductionService.h"
#include "../Services/ProductionLineService.h"
#include "../Services/ReleaseService.h"
#include "../Views/MenuView.h"
#include "../Views/SampleView.h"
#include "../Views/OrderView.h"
#include "../Views/ProductionView.h"
#include "../Views/ProductionLineView.h"
#include "../Views/MonitorView.h"
#include "../Views/ReleaseView.h"
#include "SampleController.h"
#include "OrderController.h"
#include "ProductionController.h"
#include "ReleaseController.h"
#include <vector>
#include <string>

class AppController {
public:
    AppController();
    void Run();
private:
    void HandleMenu(int choice);
    void ShowMonitor();
    void ShowHistory();
    void Tick();
    void AddLog(const std::string& msg);

    // Repositories
    SampleRepository         sampleRepo_;
    OrderRepository          orderRepo_;
    InventoryRepository      invRepo_;
    ProductionRepository     prodRepo_;
    ProductionLineRepository lineRepo_;

    // Services
    SampleService         sampleSvc_;
    OrderService          orderSvc_;
    InventoryService      invSvc_;
    ProductionService     prodSvc_;
    ProductionLineService lineSvc_;
    ReleaseService        relSvc_;

    // Views
    MenuView           menuView_;
    SampleView         sampleView_;
    OrderView          orderView_;
    ProductionView     prodView_;
    ProductionLineView lineView_;
    MonitorView        monitorView_;
    ReleaseView        releaseView_;

    // Controllers
    SampleController     sampleCtrl_;
    OrderController      orderCtrl_;
    ProductionController prodCtrl_;
    ReleaseController    releaseCtrl_;

    std::vector<std::string> eventLog_;
    bool running_ = true;
};

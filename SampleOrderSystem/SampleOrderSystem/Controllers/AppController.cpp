#include "AppController.h"
#include "../Views/ConsoleHelper.h"
#include "../Utils/Utils.h"
#include <iostream>

AppController::AppController()
    : sampleSvc_(sampleRepo_)
    , orderSvc_(orderRepo_)
    , invSvc_(invRepo_)
    , prodSvc_(prodRepo_)
    , relSvc_(orderRepo_, invSvc_)
    , sampleCtrl_(sampleSvc_, invSvc_, sampleView_)
    , orderCtrl_(orderSvc_, sampleSvc_, invSvc_, prodSvc_, orderView_,
                 [this](const std::string& msg) { AddLog(msg); })
    , prodCtrl_(prodSvc_, sampleSvc_, prodView_)
    , releaseCtrl_(relSvc_, sampleSvc_, releaseView_,
                   [this](const std::string& msg) { AddLog(msg); })
{
    prodSvc_.SetOnCompleteCallback([this](const ProductionJob& job) {
        orderSvc_.UpdateStatus(job.order_id, OrderStatus::CONFIRMED);
        invSvc_.Replenish(job.sample_id, job.produced_qty);
        AddLog("생산 완료→CONFIRMED: 주문 " + std::to_string(job.order_id)
               + "  +" + std::to_string(job.produced_qty) + "개 입고");
    });

    AddLog("시스템 시작");
}

void AppController::Run() {
    while (running_) {
        std::cout << "\n\n";
        ConsoleHelper::PrintDivider('=');
        menuView_.Render();
        int choice = menuView_.GetChoice();
        HandleMenu(choice);
    }
    std::cout << "\n  시스템을 종료합니다.\n";
}

void AppController::HandleMenu(int choice) {
    Tick();
    switch (choice) {
    case 1: sampleCtrl_.RunMenu();         break;
    case 2: orderCtrl_.PlaceOrder();       break;
    case 3: orderCtrl_.ProcessPending();   break;
    case 4: ShowMonitor();                 break;
    case 5: prodCtrl_.ShowProduction();    break;
    case 6: releaseCtrl_.ProcessRelease(); break;
    case 7: ShowHistory();                 break;
    case 0: running_ = false;              break;
    default:
        std::cout << "\n  잘못된 선택입니다.\n";
        ConsoleHelper::Pause();
        break;
    }
}

void AppController::ShowMonitor() {
    monitorView_.Render(
        orderSvc_.FindAll(),
        invSvc_.GetAll(),
        sampleSvc_.FindAll(),
        prodSvc_.GetRunningJob(),
        prodSvc_.GetQueuedJobs());
}

void AppController::ShowHistory() {
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_WHITE);
    std::cout << "========== 주문/생산/출고 이력 ==========\n";
    ConsoleHelper::ResetColor();
    ConsoleHelper::PrintDivider('-', 60);
    if (eventLog_.empty()) {
        std::cout << "  이력이 없습니다.\n";
    } else {
        for (const auto& log : eventLog_)
            std::cout << log << '\n';
    }
    ConsoleHelper::PrintDivider('=', 60);
    ConsoleHelper::Pause();
}

void AppController::Tick() {
    prodSvc_.Tick();
}

void AppController::AddLog(const std::string& msg) {
    eventLog_.push_back("[" + GetTimestamp() + "] " + msg);
    static constexpr size_t MAX_EVENT_LOG_SIZE = 100;
    if (eventLog_.size() > MAX_EVENT_LOG_SIZE)
        eventLog_.erase(eventLog_.begin());
}

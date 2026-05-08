#include "ReleaseController.h"
#include "../Views/ConsoleHelper.h"
#include <iostream>

ReleaseController::ReleaseController(ReleaseService& relSvc, SampleService& sampleSvc,
                                      ReleaseView& view, LogCallback log)
    : relSvc_(relSvc), sampleSvc_(sampleSvc), view_(view), log_(std::move(log)) {}

void ReleaseController::ProcessRelease() {
    auto confirmed = relSvc_.GetConfirmedOrders();
    view_.PrintConfirmedOrders(confirmed, sampleSvc_.FindAll());
    if (confirmed.empty()) { ConsoleHelper::Pause(); return; }

    int id = view_.GetOrderIdInput();
    if (relSvc_.Release(id)) {
        log_("출고 완료 — 주문 ID: " + std::to_string(id));
        std::cout << "  출고 처리 완료\n";
    } else {
        std::cout << "  출고 처리 실패 (CONFIRMED 상태가 아닌 주문)\n";
    }
    ConsoleHelper::Pause();
}

#include "OrderController.h"
#include "../Views/ConsoleHelper.h"
#include <iostream>

OrderController::OrderController(OrderService& orderSvc, SampleService& sampleSvc,
                                  InventoryService& invSvc, ProductionService& prodSvc,
                                  OrderView& view, LogCallback log)
    : orderSvc_(orderSvc), sampleSvc_(sampleSvc), invSvc_(invSvc),
      prodSvc_(prodSvc), view_(view), log_(std::move(log)) {}

void OrderController::PlaceOrder() {
    auto samples = sampleSvc_.FindAll();
    if (samples.empty()) {
        std::cout << "  등록된 시료가 없습니다.\n";
        ConsoleHelper::Pause();
        return;
    }
    OrderInput input = view_.GetOrderInput(samples);
    Order o = orderSvc_.PlaceOrder(input.sample_id, input.quantity, input.customer_name);
    log_("주문 접수 — ID: " + std::to_string(o.id) + "  고객사: " + o.customer_name);
    std::cout << "  주문 접수 완료 — ID: " << o.id << "  고객사: " << o.customer_name << "\n";
    ConsoleHelper::Pause();
}

void OrderController::ProcessPending() {
    auto reserved = orderSvc_.FindByStatus(OrderStatus::RESERVED);
    if (reserved.empty()) {
        std::cout << "  승인 대기 주문이 없습니다.\n";
        ConsoleHelper::Pause();
        return;
    }
    auto samples = sampleSvc_.FindAll();
    ApprovalInput input = view_.GetApprovalInput(reserved, samples);

    if (!input.approve) {
        orderSvc_.Reject(input.order_id);
        log_("주문 거절 — ID: " + std::to_string(input.order_id));
        std::cout << "  주문 거절 처리 완료\n";
        ConsoleHelper::Pause();
        return;
    }

    Order o = orderSvc_.FindById(input.order_id);
    auto sample = sampleSvc_.FindById(o.sample_id);
    if (!sample) { std::cout << "  시료 정보를 찾을 수 없습니다.\n"; ConsoleHelper::Pause(); return; }

    if (invSvc_.CheckStock(o.sample_id, o.quantity)) {
        orderSvc_.UpdateStatus(o.id, OrderStatus::CONFIRMED);
        log_("주문 승인→CONFIRMED — ID: " + std::to_string(o.id));
        std::cout << "  재고 충분 — CONFIRMED 처리 완료\n";
    } else {
        int stock    = invSvc_.GetStock(o.sample_id);
        int shortage = o.quantity - stock;
        orderSvc_.UpdateStatus(o.id, OrderStatus::PRODUCING);
        prodSvc_.Enqueue(o.id, o.sample_id, shortage,
                         sample->yield_rate, sample->avg_production_time);
        log_("주문 승인→PRODUCING — ID: " + std::to_string(o.id) + "  부족: " + std::to_string(shortage));
        std::cout << "  재고 부족 — 생산 요청 완료 (부족: " << shortage << "개)\n";
    }
    ConsoleHelper::Pause();
}

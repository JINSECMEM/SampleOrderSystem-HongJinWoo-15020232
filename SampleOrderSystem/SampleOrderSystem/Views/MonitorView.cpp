#include "MonitorView.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

static std::string sampleName(int sampleId, const std::vector<Sample>& samples) {
    for (const auto& s : samples)
        if (s.id == sampleId) return s.name;
    return "?";
}

static int countByStatus(const std::vector<Order>& orders, OrderStatus status) {
    return static_cast<int>(std::count_if(orders.begin(), orders.end(),
        [status](const Order& o) { return o.status == status; }));
}

void MonitorView::Render(const std::vector<Order>& orders,
                          const std::vector<Inventory>& inventories,
                          const std::vector<Sample>& samples,
                          const std::optional<ProductionJob>& running,
                          const std::vector<ProductionJob>& queued,
                          const std::vector<std::string>& eventLog) {
    ConsoleHelper::ClearScreen();
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_WHITE);
    std::cout << "========== 모니터링 대시보드 ==========\n";
    ConsoleHelper::ResetColor();

    // 주문 현황
    std::cout << "\n[주문 현황]\n";
    ConsoleHelper::PrintDivider('-', 50);
    std::cout << std::left
              << std::setw(12) << "RESERVED"
              << std::setw(12) << "CONFIRMED"
              << std::setw(12) << "PRODUCING"
              << std::setw(12) << "RELEASE" << '\n';
    std::cout << std::setw(12) << countByStatus(orders, OrderStatus::RESERVED)
              << std::setw(12) << countByStatus(orders, OrderStatus::CONFIRMED)
              << std::setw(12) << countByStatus(orders, OrderStatus::PRODUCING)
              << std::setw(12) << countByStatus(orders, OrderStatus::RELEASE) << '\n';

    // 재고 현황
    std::cout << "\n[재고 현황]\n";
    ConsoleHelper::PrintDivider('-', 50);
    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(14) << "시료"
              << std::setw(8)  << "재고"
              << "상태\n";
    ConsoleHelper::PrintDivider('-', 50);

    for (const auto& inv : inventories) {
        std::string status;
        WORD color = ConsoleHelper::COLOR_DEFAULT;
        if (inv.quantity == 0) {
            status = "고갈"; color = ConsoleHelper::COLOR_RED;
        } else {
            // 해당 시료 RESERVED+PRODUCING 주문 수량 합산과 비교
            int demandQty = 0;
            for (const auto& o : orders)
                if (o.sample_id == inv.sample_id &&
                    (o.status == OrderStatus::RESERVED || o.status == OrderStatus::PRODUCING))
                    demandQty += o.quantity;
            if (inv.quantity < demandQty) { status = "부족"; color = ConsoleHelper::COLOR_YELLOW; }
            else                          { status = "여유"; color = ConsoleHelper::COLOR_GREEN; }
        }
        std::cout << std::left
                  << std::setw(5)  << inv.sample_id
                  << std::setw(14) << sampleName(inv.sample_id, samples)
                  << std::setw(8)  << inv.quantity;
        ConsoleHelper::SetColor(color);
        std::cout << status << '\n';
        ConsoleHelper::ResetColor();
    }

    // 생산 라인 요약
    std::cout << "\n[생산 라인] ";
    if (running) std::cout << "생산 중: 주문 " << running->order_id
                           << " (" << running->elapsed_min << "/" << running->total_time_min << " min)";
    else         std::cout << "유휴";
    std::cout << "  |  대기: " << queued.size() << "건\n";

    // 이벤트 로그 (최근 5줄)
    std::cout << "\n[이벤트 로그]\n";
    ConsoleHelper::PrintDivider('-', 60);
    int start = static_cast<int>(eventLog.size()) - 5;
    if (start < 0) start = 0;
    for (int i = start; i < static_cast<int>(eventLog.size()); ++i)
        std::cout << eventLog[i] << '\n';

    ConsoleHelper::PrintDivider('=', 60);
    ConsoleHelper::Pause();
}

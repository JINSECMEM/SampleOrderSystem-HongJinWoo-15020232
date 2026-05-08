#include "MonitorView.h"
#include "ConsoleHelper.h"
#include "ViewHelper.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <unordered_map>

static int countByStatus(const std::vector<Order>& orders, OrderStatus status) {
    return static_cast<int>(std::count_if(orders.begin(), orders.end(),
        [status](const Order& o) { return o.status == status; }));
}

struct InventoryDisplayStatus { std::string label; WORD color; };

static InventoryDisplayStatus GetInventoryDisplayStatus(int qty, int demandQty) {
    if (qty == 0)        return {"고갈", ConsoleHelper::COLOR_RED};
    if (qty < demandQty) return {"부족", ConsoleHelper::COLOR_YELLOW};
    return                      {"여유", ConsoleHelper::COLOR_GREEN};
}

void MonitorView::Render(const std::vector<Order>& orders,
                          const std::vector<Inventory>& inventories,
                          const std::vector<Sample>& samples,
                          const std::optional<ProductionJob>& running,
                          const std::vector<ProductionJob>& queued) {
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_WHITE);
    std::cout << "========== 모니터링 대시보드 ==========\n";
    ConsoleHelper::ResetColor();

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

    std::unordered_map<int, int> demandMap;
    for (const auto& o : orders)
        if (o.status == OrderStatus::RESERVED || o.status == OrderStatus::PRODUCING)
            demandMap[o.sample_id] += o.quantity;

    std::cout << "\n[재고 현황]\n";
    ConsoleHelper::PrintDivider('-', 50);
    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(14) << "시료"
              << std::setw(8)  << "재고"
              << "상태\n";
    ConsoleHelper::PrintDivider('-', 50);

    for (const auto& inv : inventories) {
        auto it = demandMap.find(inv.sample_id);
        int demand = (it != demandMap.end()) ? it->second : 0;
        auto [label, color] = GetInventoryDisplayStatus(inv.quantity, demand);
        std::cout << std::left
                  << std::setw(5)  << inv.sample_id
                  << std::setw(14) << ViewHelper::FindSampleName(inv.sample_id, samples)
                  << std::setw(8)  << inv.quantity;
        ConsoleHelper::SetColor(color);
        std::cout << label << '\n';
        ConsoleHelper::ResetColor();
    }

    std::cout << "\n[생산 라인] ";
    if (running) std::cout << "생산 중: 주문 " << running->order_id
                           << " (" << running->elapsed_min << "/" << running->total_time_min << " min)";
    else         std::cout << "유휴";
    std::cout << "  |  대기: " << queued.size() << "건\n";

    ConsoleHelper::PrintDivider('=', 60);
    ConsoleHelper::Pause();
}

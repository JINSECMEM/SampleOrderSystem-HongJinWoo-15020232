#include "MonitorView.h"
#include "ConsoleHelper.h"
#include "ViewHelper.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <unordered_map>

struct InventoryDisplayStatus { std::string label; WORD color; };

static InventoryDisplayStatus GetInventoryDisplayStatus(int qty, int demandQty) {
    if (qty == 0)        return {"고갈", ConsoleHelper::COLOR_RED};
    if (qty < demandQty) return {"부족", ConsoleHelper::COLOR_YELLOW};
    return                      {"여유", ConsoleHelper::COLOR_GREEN};
}

static std::string findCustomer(int orderId, const std::vector<Order>& orders) {
    for (const auto& o : orders)
        if (o.id == orderId) return o.customer_name;
    return "?";
}

static WORD statusColor(OrderStatus s) {
    switch (s) {
    case OrderStatus::RESERVED:  return ConsoleHelper::COLOR_YELLOW;
    case OrderStatus::PRODUCING: return ConsoleHelper::COLOR_CYAN;
    case OrderStatus::CONFIRMED: return ConsoleHelper::COLOR_GREEN;
    case OrderStatus::RELEASE:   return ConsoleHelper::COLOR_WHITE;
    default:                     return ConsoleHelper::COLOR_DEFAULT;
    }
}

void MonitorView::Render(const std::vector<Order>& orders,
                          const std::vector<Inventory>& inventories,
                          const std::vector<Sample>& samples,
                          const std::optional<ProductionJob>& running,
                          const std::vector<ProductionJob>& queued) {
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_WHITE);
    std::cout << "========== 모니터링 대시보드 ==========\n";
    ConsoleHelper::ResetColor();

    // ── 주문 현황 ────────────────────────────────────────────
    std::cout << "\n[주문 현황]\n";
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_CYAN);
    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(14) << "고객사"
              << std::setw(14) << "시료"
              << std::setw(6)  << "수량"
              << std::setw(12) << "STATUS"
              << "접수일시\n";
    ConsoleHelper::ResetColor();
    ConsoleHelper::PrintDivider('-', 67);

    bool hasOrder = false;
    for (const auto& o : orders) {
        if (o.status == OrderStatus::REJECTED) continue;
        hasOrder = true;
        std::cout << std::left
                  << std::setw(5)  << o.id
                  << std::setw(14) << o.customer_name
                  << std::setw(14) << ViewHelper::FindSampleName(o.sample_id, samples)
                  << std::setw(6)  << o.quantity;
        ConsoleHelper::SetColor(statusColor(o.status));
        std::cout << std::setw(12) << OrderStatusStr(o.status);
        ConsoleHelper::ResetColor();
        std::cout << o.created_at << '\n';
    }
    if (!hasOrder) std::cout << "  유효한 주문 없음\n";
    ConsoleHelper::PrintDivider('-', 67);

    // ── 재고 현황 ────────────────────────────────────────────
    std::unordered_map<int, int> demandMap;
    for (const auto& o : orders)
        if (o.status == OrderStatus::RESERVED || o.status == OrderStatus::PRODUCING)
            demandMap[o.sample_id] += o.quantity;

    std::cout << "\n[재고 현황]\n";
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_CYAN);
    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(14) << "시료"
              << std::setw(8)  << "재고"
              << "상태\n";
    ConsoleHelper::ResetColor();
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
    ConsoleHelper::PrintDivider('-', 50);

    // ── 생산 라인 ────────────────────────────────────────────
    std::cout << "\n[생산 라인]\n";
    ConsoleHelper::PrintDivider('-', 67);

    std::cout << "  현재 생산 중: ";
    if (!running) {
        std::cout << "유휴\n";
    } else {
        const auto& j = *running;
        int pct = (j.total_time_min > 0)
                  ? std::min(100, j.elapsed_min * 100 / j.total_time_min)
                  : 100;
        std::cout << '\n'
                  << "    주문 " << j.order_id
                  << "  |  고객사: " << findCustomer(j.order_id, orders)
                  << "  |  시료: " << ViewHelper::FindSampleName(j.sample_id, samples)
                  << "  |  목표: " << j.target_qty << "개\n"
                  << "    진행: ";
        ConsoleHelper::SetColor(ConsoleHelper::COLOR_GREEN);
        std::cout << pct << "% (" << j.elapsed_min << " / " << j.total_time_min << " sec)\n";
        ConsoleHelper::ResetColor();
    }

    std::cout << "\n  대기 큐 — " << queued.size() << "건\n";
    int rank = 1;
    for (const auto& j : queued) {
        std::cout << "    " << rank++ << ". 주문 " << j.order_id
                  << "  |  고객사: " << findCustomer(j.order_id, orders)
                  << "  |  시료: " << ViewHelper::FindSampleName(j.sample_id, samples)
                  << "  |  목표: " << j.target_qty << "개"
                  << "  |  총 " << j.total_time_min << " sec\n";
    }

    ConsoleHelper::PrintDivider('=', 67);
    ConsoleHelper::Pause();
}

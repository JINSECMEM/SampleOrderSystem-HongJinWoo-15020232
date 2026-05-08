#include "ProductionView.h"
#include "ConsoleHelper.h"
#include "ViewHelper.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

static std::string findCustomerName(int orderId, const std::vector<Order>& orders) {
    for (const auto& o : orders)
        if (o.id == orderId) return o.customer_name;
    return "?";
}

static int findOrderQty(int orderId, const std::vector<Order>& orders) {
    for (const auto& o : orders)
        if (o.id == orderId) return o.quantity;
    return 0;
}

static void printProgressBar(int pct, int width = 24) {
    int filled = pct * width / 100;
    std::cout << '[';
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_GREEN);
    for (int i = 0; i < width; ++i)
        std::cout << (i < filled ? '#' : ' ');
    ConsoleHelper::ResetColor();
    std::cout << "] " << std::setw(3) << pct << '%';
}

void ProductionView::Render(const std::optional<ProductionJob>& running,
                             const std::vector<ProductionJob>& queued,
                             const std::vector<Sample>& samples,
                             const std::vector<Order>& orders) {
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_WHITE);
    std::cout << "\n===== 생산 라인 조회 =====\n";
    ConsoleHelper::ResetColor();
    ConsoleHelper::PrintDivider('=', 60);

    // ── 현재 생산 중 ──────────────────────────────────────
    std::cout << "\n[현재 생산 중]\n";
    ConsoleHelper::PrintDivider('-', 60);

    if (!running) {
        std::cout << "  생산 중인 작업 없음\n";
    } else {
        const auto& j = *running;
        int pct = (j.total_time_min > 0)
                  ? std::min(100, j.elapsed_min * 100 / j.total_time_min)
                  : 100;
        int orderQty = findOrderQty(j.order_id, orders);

        std::cout << "  주문 ID   : " << j.order_id << '\n'
                  << "  고객사    : " << findCustomerName(j.order_id, orders) << '\n'
                  << "  시료      : " << ViewHelper::FindSampleName(j.sample_id, samples) << '\n'
                  << "  주문 수량 : " << orderQty << " 개\n"
                  << "  생산 목표 : " << j.target_qty << " 개  (재고 부족분 기준)\n"
                  << "  생산량    : " << j.produced_qty << " / " << j.target_qty << " 개\n"
                  << "  경과 시간 : " << j.elapsed_min << " / " << j.total_time_min << " sec\n"
                  << "  진행도    : ";
        printProgressBar(pct);
        std::cout << '\n';
    }

    // ── 대기 큐 (FIFO) ────────────────────────────────────
    ConsoleHelper::PrintDivider('-', 60);
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_CYAN);
    std::cout << "[대기 큐 — " << queued.size() << "건  |  스케줄링: FIFO]\n";
    ConsoleHelper::ResetColor();
    ConsoleHelper::PrintDivider('-', 60);

    if (queued.empty()) {
        std::cout << "  대기 중인 작업 없음\n";
    } else {
        int rank = 1;
        for (const auto& j : queued) {
            int orderQty = findOrderQty(j.order_id, orders);
            std::cout << "  [" << rank++ << "] 주문 " << j.order_id
                      << "  |  " << findCustomerName(j.order_id, orders)
                      << "  |  " << ViewHelper::FindSampleName(j.sample_id, samples)
                      << "\n       주문 수량: " << orderQty << "개"
                      << "  |  생산 목표: " << j.target_qty << "개"
                      << "  |  예상 소요: " << j.total_time_min << " sec\n";
        }
    }

    ConsoleHelper::PrintDivider('=', 60);
    ConsoleHelper::Pause();
}

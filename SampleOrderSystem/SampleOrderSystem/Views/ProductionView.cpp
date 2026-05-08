#include "ProductionView.h"
#include "ConsoleHelper.h"
#include "ViewHelper.h"
#include <iostream>
#include <iomanip>

void ProductionView::Render(const std::optional<ProductionJob>& running,
                             const std::vector<ProductionJob>& queued,
                             const std::vector<Sample>& samples) {
    std::cout << "\n[생산 라인]\n";
    ConsoleHelper::PrintDivider('=', 50);

    std::cout << "[현재 생산 중]\n";
    if (!running) {
        std::cout << "  생산 중인 작업 없음\n";
    } else {
        const auto& j = *running;
        int pct = (j.total_time_min > 0)
                  ? (j.elapsed_min * 100 / j.total_time_min)
                  : 100;
        std::cout << "  주문 ID     : " << j.order_id << '\n'
                  << "  시료        : " << ViewHelper::FindSampleName(j.sample_id, samples) << '\n'
                  << "  목표 수량   : " << j.target_qty << " 개\n"
                  << "  생산량      : " << j.produced_qty << " 개\n"
                  << "  진행        : ";
        ConsoleHelper::SetColor(ConsoleHelper::COLOR_GREEN);
        std::cout << pct << "% (" << j.elapsed_min << "/" << j.total_time_min << " min)\n";
        ConsoleHelper::ResetColor();
    }

    ConsoleHelper::PrintDivider('-', 50);
    std::cout << "[대기 큐 — " << queued.size() << "건]\n";
    if (queued.empty()) {
        std::cout << "  대기 중인 작업 없음\n";
    } else {
        int rank = 1;
        for (const auto& j : queued)
            std::cout << "  " << rank++ << ". 주문 " << j.order_id
                      << "  " << ViewHelper::FindSampleName(j.sample_id, samples)
                      << " x" << j.target_qty
                      << "  (총 " << j.total_time_min << " min)\n";
    }
    ConsoleHelper::PrintDivider('=', 50);
    ConsoleHelper::Pause();
}

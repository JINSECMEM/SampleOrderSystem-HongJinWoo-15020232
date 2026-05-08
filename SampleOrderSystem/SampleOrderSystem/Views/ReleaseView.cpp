#include "ReleaseView.h"
#include "ConsoleHelper.h"
#include "ViewHelper.h"
#include <iostream>
#include <iomanip>
#include <limits>

void ReleaseView::PrintConfirmedOrders(const std::vector<Order>& orders,
                                        const std::vector<Sample>& samples) {
    std::cout << "\n[출고 대기 주문 — CONFIRMED]\n";
    ConsoleHelper::PrintDivider('-', 45);
    if (orders.empty()) { std::cout << "  출고 대기 주문 없음\n"; return; }
    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(14) << "고객사"
              << std::setw(14) << "시료"
              << std::setw(6)  << "수량"
              << "승인일시\n";
    ConsoleHelper::PrintDivider('-', 55);
    for (const auto& o : orders)
        std::cout << std::left
                  << std::setw(5)  << o.id
                  << std::setw(14) << o.customer_name
                  << std::setw(14) << ViewHelper::FindSampleName(o.sample_id, samples)
                  << std::setw(6)  << o.quantity
                  << o.updated_at << '\n';
    ConsoleHelper::PrintDivider('-', 55);
}

int ReleaseView::GetOrderIdInput() {
    std::cout << "출고할 주문 ID 입력: ";
    int id; std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return id;
}

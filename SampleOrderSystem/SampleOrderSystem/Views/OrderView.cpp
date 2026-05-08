#include "OrderView.h"
#include "ConsoleHelper.h"
#include "ViewHelper.h"
#include <iostream>
#include <iomanip>
#include <limits>

void OrderView::PrintList(const std::vector<Order>& orders, const std::vector<Sample>& samples) {
    std::cout << "\n[주문 목록]\n";
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_CYAN);
    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(14) << "시료"
              << std::setw(8)  << "수량"
              << std::setw(12) << "상태"
              << "접수일시\n";
    ConsoleHelper::ResetColor();
    ConsoleHelper::PrintDivider('-', 55);
    for (const auto& o : orders) {
        std::cout << std::left
                  << std::setw(5)  << o.id
                  << std::setw(14) << ViewHelper::FindSampleName(o.sample_id, samples)
                  << std::setw(8)  << o.quantity
                  << std::setw(12) << OrderStatusStr(o.status)
                  << o.created_at << '\n';
    }
    ConsoleHelper::PrintDivider('-', 55);
}

OrderInput OrderView::GetOrderInput(const std::vector<Sample>& samples) {
    std::cout << "\n[주문 접수]\n";
    std::cout << "등록된 시료:\n";
    for (const auto& s : samples)
        std::cout << "  [" << s.id << "] " << s.name << '\n';

    OrderInput input;
    std::cout << "시료 ID 선택: ";
    std::cin >> input.sample_id;
    std::cout << "주문 수량 입력: ";
    std::cin >> input.quantity;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return input;
}

ApprovalInput OrderView::GetApprovalInput(const std::vector<Order>& reserved,
                                           const std::vector<Sample>& samples) {
    std::cout << "\n[승인 대기 주문]\n";
    ConsoleHelper::PrintDivider('-', 40);
    for (const auto& o : reserved)
        std::cout << "  [" << o.id << "] " << ViewHelper::FindSampleName(o.sample_id, samples)
                  << " x" << o.quantity << "  (" << o.created_at << ")\n";
    ConsoleHelper::PrintDivider('-', 40);

    ApprovalInput input;
    std::cout << "처리할 주문 ID: ";
    std::cin >> input.order_id;
    std::cout << "1.승인  2.거절  선택: ";
    int ch; std::cin >> ch;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    input.approve = (ch == 1);
    return input;
}

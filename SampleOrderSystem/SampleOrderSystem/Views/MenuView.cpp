#include "MenuView.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <limits>

void MenuView::Render() {
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_WHITE);
    std::cout << "===== 반도체 시료 생산 관리 시스템 =====\n";
    ConsoleHelper::ResetColor();
    std::cout << "1. 시료 등록\n"
              << "2. 시료 조회\n"
              << "3. 시료 검색\n"
              << "4. 주문 접수\n"
              << "5. 주문 승인/거절\n"
              << "6. 모니터링\n"
              << "7. 생산 라인\n"
              << "8. 출고 처리\n"
              << "0. 종료\n";
    ConsoleHelper::PrintDivider();
    std::cout << "메뉴 선택: ";
}

int MenuView::GetChoice() {
    int ch;
    std::cin >> ch;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return ch;
}

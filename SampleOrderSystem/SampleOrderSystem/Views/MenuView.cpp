#include "MenuView.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <limits>

void MenuView::Render() {
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_WHITE);
    std::cout << "===== 반도체 시료 생산 관리 시스템 =====\n";
    ConsoleHelper::ResetColor();
    std::cout << "1. 시료 관리\n"
              << "2. 주문 접수\n"
              << "3. 주문 승인/거절\n"
              << "4. 모니터링\n"
              << "5. 생산 라인 조회\n"
              << "6. 출고 처리\n"
              << "7. 주문/생산/출고 이력\n"
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

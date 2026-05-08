#include "ProductionLineView.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <iomanip>
#include <limits>

void ProductionLineView::RenderList(const std::vector<ProductionLine>& lines,
                                     const std::vector<ProductionJob>&  runningJobs) {
    std::cout << "\n[생산 라인 목록]\n";
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_CYAN);
    std::cout << std::left << std::setw(5) << "ID" << std::setw(16) << "이름" << "상태\n";
    ConsoleHelper::ResetColor();
    ConsoleHelper::PrintDivider('-', 40);
    for (const auto& l : lines) {
        bool running = false;
        for (const auto& j : runningJobs)
            if (j.line_id == l.id) { running = true; break; }
        std::cout << std::left << std::setw(5) << l.id << std::setw(16) << l.name;
        if (running) { ConsoleHelper::SetColor(ConsoleHelper::COLOR_GREEN); std::cout << "생산 중\n"; }
        else         { std::cout << "유휴\n"; }
        ConsoleHelper::ResetColor();
    }
    ConsoleHelper::PrintDivider('-', 40);
}

void ProductionLineView::RenderSubMenu() {
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_WHITE);
    std::cout << "===== 생산 라인 관리 =====\n";
    ConsoleHelper::ResetColor();
    std::cout << "1. 라인 목록 조회\n"
              << "2. 생산 라인 추가\n"
              << "3. 생산 라인 삭제\n"
              << "0. 뒤로\n";
    ConsoleHelper::PrintDivider();
    std::cout << "메뉴 선택: ";
}

int ProductionLineView::GetSubChoice() {
    int ch; std::cin >> ch;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return ch;
}

std::string ProductionLineView::GetLineName() {
    std::cout << "생산 라인 이름 입력: ";
    std::string name; std::getline(std::cin, name);
    return name;
}

int ProductionLineView::GetLineId(const std::vector<ProductionLine>& lines) {
    RenderList(lines, {});
    std::cout << "삭제할 라인 ID 입력: ";
    int id; std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return id;
}

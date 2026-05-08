#include "SampleController.h"
#include "../Views/ConsoleHelper.h"
#include <iostream>
#include <limits>

SampleController::SampleController(SampleService& sampleSvc, InventoryService& invSvc, SampleView& view)
    : sampleSvc_(sampleSvc), invSvc_(invSvc), view_(view) {}

void SampleController::RunMenu() {
    while (true) {
        ConsoleHelper::ClearScreen();
        ConsoleHelper::SetColor(ConsoleHelper::COLOR_WHITE);
        std::cout << "===== 시료 관리 =====\n";
        ConsoleHelper::ResetColor();
        std::cout << "1. 시료 등록\n"
                  << "2. 시료 목록\n"
                  << "3. 시료 검색\n"
                  << "0. 뒤로\n";
        ConsoleHelper::PrintDivider();
        std::cout << "메뉴 선택: ";

        int ch;
        std::cin >> ch;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (ch) {
        case 1: Register(); break;
        case 2: ListAll();  break;
        case 3: Search();   break;
        case 0: return;
        default:
            std::cout << "  잘못된 선택입니다.\n";
            ConsoleHelper::Pause();
            break;
        }
    }
}

void SampleController::Register() {
    std::string name   = view_.GetNameInput();
    int avgTime        = view_.GetAvgTimeInput();
    double yieldRate   = view_.GetYieldRateInput();
    Sample s = sampleSvc_.Register(name, avgTime, yieldRate);
    std::cout << "  시료 등록 완료 — ID: " << s.id << "\n";
    ConsoleHelper::Pause();
}

void SampleController::Search() {
    std::string kw = view_.GetKeywordInput();
    view_.PrintSearchResult(sampleSvc_.SearchByName(kw));
}

void SampleController::ListAll() {
    view_.PrintList(sampleSvc_.FindAll());
}

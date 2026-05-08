#include "SampleController.h"
#include "../Views/ConsoleHelper.h"
#include <iostream>
#include <limits>

SampleController::SampleController(SampleService& sampleSvc, InventoryService& invSvc, SampleView& view)
    : sampleSvc_(sampleSvc), invSvc_(invSvc), view_(view) {}

void SampleController::RunMenu() {
    while (true) {
        std::cout << "\n\n";
        ConsoleHelper::PrintDivider('=');
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
    std::string name  = view_.GetNameInput();
    int avgTime       = view_.GetAvgTimeInput();
    double yieldRate  = view_.GetYieldRateInput();
    auto result = sampleSvc_.Register(name, avgTime, yieldRate);
    if (!result) {
        std::cout << "  등록 실패: '" << name << "'은(는) 이미 등록된 시료입니다.\n";
        ConsoleHelper::Pause();
        return;
    }
    std::cout << "  시료 등록 완료 — ID: " << result->id << "\n\n";
    view_.DisplayList(sampleSvc_.FindAll());
    ConsoleHelper::Pause();
}

void SampleController::Search() {
    while (true) {
        std::cout << "\n\n";
        ConsoleHelper::PrintDivider('=');
        view_.DisplayList(sampleSvc_.FindAll());

        std::cout << "\n[검색 조건 선택]\n"
                  << "1. 이름으로 검색\n"
                  << "2. 생산시간으로 검색 (N분 이하)\n"
                  << "3. 수율로 검색 (N% 이상)\n"
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
        case 1:
            view_.PrintSearchResult(sampleSvc_.SearchByName(view_.GetKeywordInput()));
            break;
        case 2:
            view_.PrintSearchResult(sampleSvc_.SearchByMaxTime(view_.GetMaxTimeInput()));
            break;
        case 3:
            view_.PrintSearchResult(sampleSvc_.SearchByMinYield(view_.GetMinYieldInput()));
            break;
        case 0: return;
        default:
            std::cout << "  잘못된 선택입니다.\n";
            ConsoleHelper::Pause();
            break;
        }
    }
}

void SampleController::ListAll() {
    view_.PrintList(sampleSvc_.FindAll());
}

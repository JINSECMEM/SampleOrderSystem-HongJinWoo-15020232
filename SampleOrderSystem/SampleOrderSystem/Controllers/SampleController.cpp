#include "SampleController.h"
#include "../Views/ConsoleHelper.h"
#include <iostream>

SampleController::SampleController(SampleService& sampleSvc, InventoryService& invSvc, SampleView& view)
    : sampleSvc_(sampleSvc), invSvc_(invSvc), view_(view) {}

void SampleController::Register() {
    std::string name   = view_.GetNameInput();
    int avgTime        = view_.GetAvgTimeInput();
    double yieldRate   = view_.GetYieldRateInput();
    Sample s = sampleSvc_.Register(name, avgTime, yieldRate);
    std::cout << "  시료 등록 완료 — ID: " << s.id << "\n";
    ConsoleHelper::Pause();
}

void SampleController::FindById() {
    int id = view_.GetIdInput();
    auto result = sampleSvc_.FindById(id);
    if (!result) {
        std::cout << "  ID " << id << "에 해당하는 시료가 없습니다.\n";
        ConsoleHelper::Pause();
        return;
    }
    view_.PrintDetail(*result, invSvc_.GetStock(result->id));
}

void SampleController::Search() {
    std::string kw = view_.GetKeywordInput();
    view_.PrintSearchResult(sampleSvc_.SearchByName(kw));
}

void SampleController::ListAll() {
    view_.PrintList(sampleSvc_.FindAll());
}

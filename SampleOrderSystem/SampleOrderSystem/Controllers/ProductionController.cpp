#include "ProductionController.h"
#include "../Views/ConsoleHelper.h"
#include <iostream>
#include <limits>

ProductionController::ProductionController(ProductionService& prodSvc,
                                            ProductionLineService& lineSvc,
                                            SampleService& sampleSvc,
                                            OrderService& orderSvc,
                                            ProductionView& view,
                                            ProductionLineView& lineView)
    : prodSvc_(prodSvc), lineSvc_(lineSvc), sampleSvc_(sampleSvc),
      orderSvc_(orderSvc), view_(view), lineView_(lineView) {}

void ProductionController::ShowProduction() {
    while (true) {
        std::cout << "\n\n";
        ConsoleHelper::PrintDivider('=');
        lineView_.RenderSubMenu();
        int ch = lineView_.GetSubChoice();
        switch (ch) {
        case 1: ShowLines();  break;
        case 2: AddLine();    break;
        case 3: DeleteLine(); break;
        case 0: return;
        default:
            std::cout << "  잘못된 선택입니다.\n";
            ConsoleHelper::Pause();
        }
    }
}

void ProductionController::ShowLines() {
    view_.Render(lineSvc_.GetAllLines(),
                 prodSvc_.GetAllRunningJobs(),
                 prodSvc_.GetQueuedJobs(),
                 sampleSvc_.FindAll(),
                 orderSvc_.FindAll());
}

void ProductionController::AddLine() {
    std::string name = lineView_.GetLineName();
    if (name.empty()) {
        std::cout << "  이름을 입력해주세요.\n";
        ConsoleHelper::Pause();
        return;
    }
    auto line = lineSvc_.AddLine(name);
    std::cout << "  생산 라인 추가 완료 — ID: " << line.id << "  이름: " << line.name << "\n";
    ConsoleHelper::Pause();
}

void ProductionController::DeleteLine() {
    auto lines = lineSvc_.GetAllLines();
    if (lines.empty()) {
        std::cout << "  등록된 생산 라인이 없습니다.\n";
        ConsoleHelper::Pause();
        return;
    }
    int id = lineView_.GetLineId(lines);
    if (lineSvc_.DeleteLine(id)) {
        std::cout << "  생산 라인 삭제 완료 — ID: " << id << "\n";
    } else {
        std::cout << "  삭제 실패: 해당 라인에 생산 중인 작업이 있습니다.\n";
    }
    ConsoleHelper::Pause();
}

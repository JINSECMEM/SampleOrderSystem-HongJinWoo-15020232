#include "SampleView.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <iomanip>
#include <limits>

static void printHeader() {
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_CYAN);
    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(14) << "이름"
              << std::setw(16) << "생산시간(min)"
              << std::setw(10) << "수율(%)"
              << '\n';
    ConsoleHelper::ResetColor();
    ConsoleHelper::PrintDivider('-', 45);
}

static void printRow(const Sample& s) {
    std::cout << std::left
              << std::setw(5)  << s.id
              << std::setw(14) << s.name
              << std::setw(16) << s.avg_production_time
              << std::fixed << std::setprecision(2)
              << std::setw(10) << (s.yield_rate * 100.0)
              << '\n';
}

void SampleView::PrintList(const std::vector<Sample>& samples) {
    std::cout << "\n[시료 목록]\n";
    printHeader();
    for (const auto& s : samples) printRow(s);
    ConsoleHelper::PrintDivider('-', 45);
    std::cout << "총 " << samples.size() << "개\n";
    ConsoleHelper::Pause();
}

void SampleView::PrintDetail(const Sample& s, int stock) {
    std::cout << "\n[시료 상세]\n";
    ConsoleHelper::PrintDivider('-', 40);
    std::cout << "  ID            : " << s.id << '\n'
              << "  이름          : " << s.name << '\n'
              << "  평균 생산시간 : " << s.avg_production_time << " min\n"
              << std::fixed << std::setprecision(2)
              << "  수율          : " << (s.yield_rate * 100.0) << " %\n"
              << "  현재 재고     : " << stock << " 개\n";
    ConsoleHelper::PrintDivider('-', 40);
    ConsoleHelper::Pause();
}

void SampleView::PrintSearchResult(const std::vector<Sample>& samples) {
    std::cout << "\n[검색 결과 — " << samples.size() << "건]\n";
    if (samples.empty()) { std::cout << "  결과 없음\n"; ConsoleHelper::Pause(); return; }
    printHeader();
    for (const auto& s : samples) printRow(s);
    ConsoleHelper::Pause();
}

std::string SampleView::GetNameInput() {
    std::cout << "시료 이름 입력: ";
    std::string name;
    std::getline(std::cin, name);
    return name;
}

int SampleView::GetAvgTimeInput() {
    std::cout << "평균 생산시간(분) 입력: ";
    int v; std::cin >> v;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return v;
}

double SampleView::GetYieldRateInput() {
    std::cout << "수율 입력(0.0~1.0): ";
    double v; std::cin >> v;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return v;
}

int SampleView::GetIdInput() {
    std::cout << "시료 ID 입력: ";
    int v; std::cin >> v;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return v;
}

std::string SampleView::GetKeywordInput() {
    std::cout << "검색 키워드 입력: ";
    std::string kw;
    std::getline(std::cin, kw);
    return kw;
}

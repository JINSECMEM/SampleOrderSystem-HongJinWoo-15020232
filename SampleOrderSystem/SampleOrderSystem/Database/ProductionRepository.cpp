#include "ProductionRepository.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <direct.h>

static void ensureDataDir() { _mkdir("data"); }

static JobStatus ParseJobStatus(const std::string& s) {
    if (s == "RUNNING")   return JobStatus::RUNNING;
    if (s == "COMPLETED") return JobStatus::COMPLETED;
    return JobStatus::QUEUED;
}
static const char* JobStatusStr(JobStatus s) {
    switch (s) {
    case JobStatus::RUNNING:   return "RUNNING";
    case JobStatus::COMPLETED: return "COMPLETED";
    default:                   return "QUEUED";
    }
}

ProductionRepository::ProductionRepository() {
    ensureDataDir();
    LoadFromFile();
}

ProductionJob ProductionRepository::SaveJob(const ProductionJob& j) {
    ProductionJob copy = j;
    copy.id = nextId_++;
    store_[copy.id] = copy;
    SaveToFile();
    return copy;
}

ProductionJob ProductionRepository::FindJobById(int id) {
    auto it = store_.find(id);
    if (it == store_.end()) throw std::runtime_error("ProductionJob not found: " + std::to_string(id));
    return it->second;
}

std::vector<ProductionJob> ProductionRepository::FindAllJobs() {
    std::vector<ProductionJob> result;
    for (const auto& [id, j] : store_) result.push_back(j);
    return result;
}

void ProductionRepository::UpdateJob(const ProductionJob& j) {
    store_[j.id] = j;
}

std::optional<ProductionJob> ProductionRepository::FindNextQueued() {
    // line_id==0 && QUEUED, id ASC (FIFO)
    for (const auto& [id, j] : store_)
        if (j.status == JobStatus::QUEUED && j.line_id == 0) return j;
    return std::nullopt;
}

std::optional<ProductionJob> ProductionRepository::FindRunningJobByLine(int lineId) {
    for (const auto& [id, j] : store_)
        if (j.status == JobStatus::RUNNING && j.line_id == lineId) return j;
    return std::nullopt;
}

std::vector<ProductionJob> ProductionRepository::FindAllRunningJobs() {
    std::vector<ProductionJob> result;
    for (const auto& [id, j] : store_)
        if (j.status == JobStatus::RUNNING) result.push_back(j);
    return result;
}

bool ProductionRepository::HasRunningJobOnLine(int lineId) const {
    for (const auto& [id, j] : store_)
        if (j.status == JobStatus::RUNNING && j.line_id == lineId) return true;
    return false;
}

void ProductionRepository::SaveToFile() const {
    std::ofstream f(DATA_FILE);
    for (const auto& [id, j] : store_) {
        f << j.id         << '|' << j.order_id      << '|' << j.sample_id    << '|'
          << j.line_id    << '|' << j.target_qty     << '|' << j.produced_qty << '|'
          << j.total_time_min << '|' << j.elapsed_min << '|'
          << static_cast<long long>(j.start_time) << '|'
          << JobStatusStr(j.status) << '|'
          << j.created_at << '|' << j.completed_at << '\n';
    }
}

void ProductionRepository::LoadFromFile() {
    std::ifstream f(DATA_FILE);
    if (!f.is_open()) return;
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream ss(line);
        std::string tok;
        ProductionJob j;
        if (std::getline(ss, tok, '|')) j.id            = std::stoi(tok);
        if (std::getline(ss, tok, '|')) j.order_id      = std::stoi(tok);
        if (std::getline(ss, tok, '|')) j.sample_id     = std::stoi(tok);
        if (std::getline(ss, tok, '|')) j.line_id       = std::stoi(tok);
        if (std::getline(ss, tok, '|')) j.target_qty    = std::stoi(tok);
        if (std::getline(ss, tok, '|')) j.produced_qty  = std::stoi(tok);
        if (std::getline(ss, tok, '|')) j.total_time_min = std::stoi(tok);
        if (std::getline(ss, tok, '|')) j.elapsed_min   = std::stoi(tok);
        if (std::getline(ss, tok, '|')) j.start_time    = static_cast<std::time_t>(std::stoll(tok));
        if (std::getline(ss, tok, '|')) j.status        = ParseJobStatus(tok);
        if (std::getline(ss, tok, '|')) j.created_at    = tok;
        if (std::getline(ss, tok))      j.completed_at  = tok;
        store_[j.id] = j;
        if (j.id >= nextId_) nextId_ = j.id + 1;
    }
}

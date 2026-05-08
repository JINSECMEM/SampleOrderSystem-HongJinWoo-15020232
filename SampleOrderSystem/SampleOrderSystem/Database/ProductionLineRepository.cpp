#include "ProductionLineRepository.h"
#include "../Utils/Utils.h"
#include <fstream>
#include <sstream>
#include <direct.h>  // _mkdir (Windows)

static void ensureDataDir() { _mkdir("data"); }

ProductionLineRepository::ProductionLineRepository() {
    ensureDataDir();
    LoadFromFile();
    if (store_.empty()) {
        ProductionLine def; def.name = "Line-1"; def.created_at = GetTimestamp();
        Save(def);
    }
}

ProductionLine ProductionLineRepository::Save(const ProductionLine& l) {
    ProductionLine copy = l;
    copy.id = nextId_++;
    store_[copy.id] = copy;
    SaveToFile();
    return copy;
}

std::optional<ProductionLine> ProductionLineRepository::FindById(int id) {
    auto it = store_.find(id);
    return it == store_.end() ? std::nullopt : std::optional<ProductionLine>{it->second};
}

std::vector<ProductionLine> ProductionLineRepository::FindAll() {
    std::vector<ProductionLine> result;
    for (const auto& [id, l] : store_) result.push_back(l);
    return result;
}

void ProductionLineRepository::Delete(int id) {
    store_.erase(id);
    SaveToFile();
}

void ProductionLineRepository::SaveToFile() const {
    std::ofstream f(DATA_FILE);
    for (const auto& [id, l] : store_)
        f << l.id << '|' << l.name << '|' << l.created_at << '\n';
}

void ProductionLineRepository::LoadFromFile() {
    std::ifstream f(DATA_FILE);
    if (!f.is_open()) return;
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream ss(line);
        std::string tok;
        ProductionLine l;
        if (std::getline(ss, tok, '|')) l.id          = std::stoi(tok);
        if (std::getline(ss, tok, '|')) l.name        = tok;
        if (std::getline(ss, tok))      l.created_at  = tok;
        store_[l.id] = l;
        if (l.id >= nextId_) nextId_ = l.id + 1;
    }
}

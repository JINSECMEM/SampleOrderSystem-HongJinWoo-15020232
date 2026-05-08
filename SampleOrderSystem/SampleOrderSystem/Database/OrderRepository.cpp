#include "OrderRepository.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <direct.h>

static void ensureDataDir() { _mkdir("data"); }

static OrderStatus ParseOrderStatus(const std::string& s) {
    if (s == "REJECTED")  return OrderStatus::REJECTED;
    if (s == "CONFIRMED") return OrderStatus::CONFIRMED;
    if (s == "PRODUCING") return OrderStatus::PRODUCING;
    if (s == "RELEASE")   return OrderStatus::RELEASE;
    return OrderStatus::RESERVED;
}

OrderRepository::OrderRepository() {
    ensureDataDir();
    LoadFromFile();
}

Order OrderRepository::Save(const Order& o) {
    Order copy = o;
    copy.id = nextId_++;
    store_[copy.id] = copy;
    SaveToFile();
    return copy;
}

Order OrderRepository::FindById(int id) {
    auto it = store_.find(id);
    if (it == store_.end()) throw std::runtime_error("Order not found: " + std::to_string(id));
    return it->second;
}

std::vector<Order> OrderRepository::FindAll() {
    std::vector<Order> result;
    for (const auto& [id, o] : store_) result.push_back(o);
    return result;
}

void OrderRepository::Update(const Order& o) {
    store_[o.id] = o;
    SaveToFile();
}

std::vector<Order> OrderRepository::FindByStatus(OrderStatus status) {
    std::vector<Order> result;
    for (const auto& [id, o] : store_)
        if (o.status == status) result.push_back(o);
    return result;
}

void OrderRepository::SaveToFile() const {
    std::ofstream f(DATA_FILE);
    for (const auto& [id, o] : store_) {
        f << o.id            << '|' << o.sample_id     << '|'
          << o.quantity      << '|' << o.customer_name  << '|'
          << OrderStatusStr(o.status) << '|'
          << o.created_at    << '|' << o.updated_at    << '\n';
    }
}

void OrderRepository::LoadFromFile() {
    std::ifstream f(DATA_FILE);
    if (!f.is_open()) return;
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream ss(line);
        std::string tok;
        Order o;
        if (std::getline(ss, tok, '|')) o.id            = std::stoi(tok);
        if (std::getline(ss, tok, '|')) o.sample_id     = std::stoi(tok);
        if (std::getline(ss, tok, '|')) o.quantity      = std::stoi(tok);
        if (std::getline(ss, tok, '|')) o.customer_name = tok;
        if (std::getline(ss, tok, '|')) o.status        = ParseOrderStatus(tok);
        if (std::getline(ss, tok, '|')) o.created_at    = tok;
        if (std::getline(ss, tok))      o.updated_at    = tok;
        store_[o.id] = o;
        if (o.id >= nextId_) nextId_ = o.id + 1;
    }
}

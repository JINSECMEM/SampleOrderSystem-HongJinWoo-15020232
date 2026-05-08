#pragma once
#include <gmock/gmock.h>
#include "Database/IInventoryRepository.h"

class MockInventoryRepository : public IInventoryRepository {
public:
    MOCK_METHOD(void,                        Save,            (const Inventory&), (override));
    MOCK_METHOD(std::optional<Inventory>,    FindBySampleId,  (int),              (override));
    MOCK_METHOD(std::vector<Inventory>,      FindAll,         (),                 (override));
    MOCK_METHOD(void,                        Update,          (const Inventory&), (override));
};

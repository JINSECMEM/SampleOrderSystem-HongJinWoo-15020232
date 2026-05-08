#pragma once
#include <gmock/gmock.h>
#include "Database/IProductionLineRepository.h"

class MockProductionLineRepository : public IProductionLineRepository {
public:
    MOCK_METHOD(ProductionLine,                  Save,         (const ProductionLine&), (override));
    MOCK_METHOD(std::optional<ProductionLine>,   FindById,     (int),                  (override));
    MOCK_METHOD(std::vector<ProductionLine>,      FindAll,      (),                     (override));
    MOCK_METHOD(void,                            Delete,       (int),                  (override));
    MOCK_METHOD(void,                            SaveToFile,   (),                     (const, override));
};

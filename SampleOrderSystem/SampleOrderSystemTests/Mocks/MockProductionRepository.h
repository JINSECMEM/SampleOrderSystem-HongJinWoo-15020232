#pragma once
#include <gmock/gmock.h>
#include "Database/IProductionRepository.h"

class MockProductionRepository : public IProductionRepository {
public:
    MOCK_METHOD(ProductionJob,                   SaveJob,               (const ProductionJob&),  (override));
    MOCK_METHOD(ProductionJob,                   FindJobById,           (int),                   (override));
    MOCK_METHOD(std::vector<ProductionJob>,       FindAllJobs,           (),                      (override));
    MOCK_METHOD(void,                            UpdateJob,             (const ProductionJob&),  (override));
    MOCK_METHOD(std::optional<ProductionJob>,    FindNextQueued,        (),                      (override));
    MOCK_METHOD(std::optional<ProductionJob>,    FindRunningJobByLine,  (int),                   (override));
    MOCK_METHOD(std::vector<ProductionJob>,       FindAllRunningJobs,    (),                      (override));
    MOCK_METHOD(bool,                            HasRunningJobOnLine,   (int),                   (const, override));
    MOCK_METHOD(void,                            SaveToFile,            (),                      (const, override));
};

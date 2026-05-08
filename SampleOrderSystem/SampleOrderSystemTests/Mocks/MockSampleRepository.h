#pragma once
#include <gmock/gmock.h>
#include "Database/ISampleRepository.h"

class MockSampleRepository : public ISampleRepository {
public:
    MOCK_METHOD(Sample,                    Save,     (const Sample&), (override));
    MOCK_METHOD(std::optional<Sample>,     FindById, (int),           (override));
    MOCK_METHOD(std::vector<Sample>,       FindAll,  (),              (override));
    MOCK_METHOD(void,                      Update,   (const Sample&), (override));
};

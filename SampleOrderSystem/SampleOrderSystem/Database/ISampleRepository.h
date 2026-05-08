#pragma once
#include <vector>
#include <optional>
#include "../Models/Sample.h"

class ISampleRepository {
public:
    virtual ~ISampleRepository() = default;
    virtual Sample                    Save(const Sample& s)   = 0;
    virtual std::optional<Sample>     FindById(int id)        = 0;
    virtual std::vector<Sample>       FindAll()               = 0;
    virtual void                      Update(const Sample& s) = 0;
};

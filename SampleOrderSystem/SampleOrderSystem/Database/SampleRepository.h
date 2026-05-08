#pragma once
#include "ISampleRepository.h"
#include <map>

class SampleRepository : public ISampleRepository {
public:
    SampleRepository();
    Sample                Save(const Sample& s)   override;
    std::optional<Sample> FindById(int id)        override;
    std::vector<Sample>   FindAll()               override;
    void                  Update(const Sample& s) override;
private:
    std::map<int, Sample> store_;
    int nextId_ = 1;
};

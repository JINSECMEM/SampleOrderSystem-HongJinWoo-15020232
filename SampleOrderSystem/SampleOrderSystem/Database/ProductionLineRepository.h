#pragma once
#include "IProductionLineRepository.h"
#include <map>

class ProductionLineRepository : public IProductionLineRepository {
public:
    ProductionLineRepository();
    ProductionLine                Save(const ProductionLine& l)   override;
    std::optional<ProductionLine> FindById(int id)                override;
    std::vector<ProductionLine>   FindAll()                       override;
    void                          Delete(int id)                  override;
    void                          SaveToFile()   const            override;
private:
    static constexpr const char* DATA_FILE = "data/production_lines.dat";
    void LoadFromFile();
    std::map<int, ProductionLine> store_;
    int nextId_ = 1;
};

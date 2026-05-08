#pragma once
#include <vector>
#include <string>
#include "../Models/ProductionLine.h"
#include "../Models/ProductionJob.h"

class ProductionLineView {
public:
    void RenderList(const std::vector<ProductionLine>& lines,
                    const std::vector<ProductionJob>&  runningJobs);
    void RenderSubMenu();
    int  GetSubChoice();
    std::string GetLineName();
    int  GetLineId(const std::vector<ProductionLine>& lines);
};

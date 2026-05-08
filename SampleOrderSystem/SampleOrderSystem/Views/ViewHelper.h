#pragma once
#include <vector>
#include <string>
#include "../Models/Sample.h"

namespace ViewHelper {
    inline std::string FindSampleName(int sampleId, const std::vector<Sample>& samples) {
        for (const auto& s : samples)
            if (s.id == sampleId) return s.name;
        return "?";
    }
}

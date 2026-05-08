#pragma once
#include <functional>
#include <string>
#include "../Services/ReleaseService.h"
#include "../Services/SampleService.h"
#include "../Views/ReleaseView.h"

class ReleaseController {
public:
    using LogCallback = std::function<void(const std::string&)>;

    ReleaseController(ReleaseService& relSvc, SampleService& sampleSvc,
                      ReleaseView& view, LogCallback log);
    void ProcessRelease();
private:
    ReleaseService& relSvc_;
    SampleService&  sampleSvc_;
    ReleaseView&    view_;
    LogCallback     log_;
};

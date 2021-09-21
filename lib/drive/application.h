#pragma once

#include <axis.h>
#include <display_reporter.h>
#include <serial_reporter.h>
#include <definitions/request.h>
#include <memory>

class Application
{
    private:
        const std::unique_ptr<Axis> xAxis;
        const std::unique_ptr<Axis> yAxis;
        const std::shared_ptr<DisplayReporter> displayReporter;
        const std::shared_ptr<SerialReporter> serialReporter;

        static void Report(void *parameter);
    public:
        Application();
        void Handle(RequestBase request);
        void Tick();
};

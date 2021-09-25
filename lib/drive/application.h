#pragma once

#include <axis.h>
#include <position_changed_producer.h>
#include <definitions/request.h>
#include <memory>

class Application
{
    private:
        const std::unique_ptr<Axis> xAxis;
        const std::unique_ptr<Axis> yAxis;
        const std::shared_ptr<PositionChangedProducer> displayProducer;
        const std::shared_ptr<PositionChangedProducer> serialProducer;

        static void Report(void *parameter);
    public:
        Application();
        void Handle(RequestBase request);
        void Tick();
};

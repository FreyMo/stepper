#pragma once

#include <axis.h>
#include <position_changed_producer.h>
#include <error_occured_producer.h>
#include <definitions/request.h>
#include <definitions/response.h>
#include <memory>
#include <map>
#include <functional>

using namespace std;

class Application
{
    private:
        const unique_ptr<Axis> xAxis;
        const unique_ptr<Axis> yAxis;
        const shared_ptr<PositionChangedProducer> displayProducer;
        const shared_ptr<PositionChangedProducer> serialProducer;
        const shared_ptr<ErrorOccuredProducer> errorProducer;

        std::map<String, function<unique_ptr<Response>(shared_ptr<RequestBase>)>> actions;

        unique_ptr<Response> HandleReferenceRequest(shared_ptr<RequestBase> request);
        unique_ptr<Response> HandleDriveToRequest(shared_ptr<RequestBase> request);

        void SendErrorOccuredMessage(String errorMessage);
        void SendPositionChangedMessage(float xAxisPosition, float yAxisPosition);
        
    public:
        Application();
        unique_ptr<Response> Handle(shared_ptr<RequestBase> request);
        void Tick();
};

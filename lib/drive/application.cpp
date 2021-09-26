#include <application.h>
#include <memory>
#include <message_queues.h>

#define X_ENABLE_PIN       GPIO_NUM_16
#define X_DIRECTION_PIN    GPIO_NUM_4
#define X_STEP_PIN         GPIO_NUM_17
#define X_LIMIT_SWITCH_PIN GPIO_NUM_13

#define Y_ENABLE_PIN       GPIO_NUM_32
#define Y_DIRECTION_PIN    GPIO_NUM_33
#define Y_STEP_PIN         GPIO_NUM_34
#define Y_LIMIT_SWITCH_PIN GPIO_NUM_35

using namespace std;

unique_ptr<Axis> setupXAxis()
{
    auto drivePins = DrivePins(X_ENABLE_PIN, X_DIRECTION_PIN, X_STEP_PIN);
    auto drive = unique_ptr<Drive>(new Drive(drivePins));

    return unique_ptr<Axis>(new Axis(move(drive), AxisPins(X_LIMIT_SWITCH_PIN)));
}

unique_ptr<Axis> setupYAxis()
{
    auto drivePins = DrivePins(Y_ENABLE_PIN, Y_DIRECTION_PIN, Y_STEP_PIN);
    auto drive = unique_ptr<Drive>(new Drive(drivePins));

    return unique_ptr<Axis>(new Axis(move(drive), AxisPins(Y_LIMIT_SWITCH_PIN)));
}

unique_ptr<Response> Application::HandleReferenceRequest(shared_ptr<RequestBase> requestBase)
{
    auto request = static_cast<ReferenceRequest*>(requestBase.get());

    if (request->payload.axis.equalsIgnoreCase("x"))
    {
        auto wasAccepted = xAxis->Reference();
        auto responseStatus = wasAccepted ? ResponseStatus::ok : ResponseStatus::error;

        return unique_ptr<Response>(new Response(request->id, responseStatus));
    }
    else if (request->payload.axis.equalsIgnoreCase("y"))
    {
        auto wasAccepted = yAxis->Reference();
        auto responseStatus = wasAccepted ? ResponseStatus::ok : ResponseStatus::error;

        return unique_ptr<Response>(new Response(request->id, responseStatus));
    }

    return unique_ptr<Response>(new Response(request->id, ResponseStatus::error));    
}

unique_ptr<Response> Application::HandleDriveToRequest(shared_ptr<RequestBase> requestBase)
{
    auto request = static_cast<DriveToRequest*>(requestBase.get());

    if (request->payload.axis.equalsIgnoreCase("x"))
    {
        auto wasAccepted = xAxis->DriveTo(request->payload.position);
        auto responseStatus = wasAccepted ? ResponseStatus::ok : ResponseStatus::error;

        return unique_ptr<Response>(new Response(request->id, responseStatus));
    }
    else if (request->payload.axis.equalsIgnoreCase("y"))
    {
        auto wasAccepted = yAxis->DriveTo(request->payload.position);
        auto responseStatus = wasAccepted ? ResponseStatus::ok : ResponseStatus::error;

        return unique_ptr<Response>(new Response(request->id, responseStatus));
    }

    return unique_ptr<Response>(new Response(request->id, ResponseStatus::ok));
}

Application::Application() :
    xAxis(setupXAxis()),
    yAxis(setupYAxis()),
    displayProducer(shared_ptr<PositionChangedProducer>(new PositionChangedProducer(displayPositionChangedQueue, 60.0f))),
    serialProducer(shared_ptr<PositionChangedProducer>(new PositionChangedProducer(serialPositionChangedQueue, 1.0f))),
    errorProducer(shared_ptr<ErrorOccuredProducer>(new ErrorOccuredProducer(errorOccuredQueue, 1.0f)))
{
    actions =
    {
        { ReferenceRequest::actionString, [this](shared_ptr<RequestBase> request) { return this->HandleReferenceRequest(request); } },
        { DriveToRequest::actionString, [this](shared_ptr<RequestBase> request) { return this->HandleDriveToRequest(request); } },
    };
}
 
unique_ptr<Response> Application::Handle(shared_ptr<RequestBase> request)
{
    auto action = actions.find(request->action);
    if (action != actions.end())
    {
        return action->second(request);
    }

    return unique_ptr<Response>(new Response(request->id, ResponseStatus::error));
}

void Application::Tick()
{
    auto xAxisPosition = xAxis->Tick();
    auto yAxisPosition = yAxis->Tick();

    SendPositionChangedMessage(xAxisPosition, yAxisPosition);
}

void Application::SendErrorOccuredMessage(String errorMessage)
{
    auto error = std::shared_ptr<ErrorOccurredMessage>(new ErrorOccurredMessage());
    error->payload.error = errorMessage;
    errorProducer->Produce(error, true);
}

void Application::SendPositionChangedMessage(float xAxisPosition, float yAxisPosition)
{
    auto positionChanged = std::shared_ptr<PositionChangedMessage>(new PositionChangedMessage());

    positionChanged->payload.xAxis = xAxisPosition;
    positionChanged->payload.yAxis = yAxisPosition;

    displayProducer->Produce(positionChanged);
    serialProducer->Produce(positionChanged);
}

#include <application.h>
#include <memory>
#include <message_queues.h>

using namespace std;

float counter = 1;

unique_ptr<Axis> setupXAxis()
{
    auto drivePins = DrivePins(GPIO_NUM_16, GPIO_NUM_4, GPIO_NUM_17);
    auto driveSettings = DriveSettings(800, 50, 3200);
    auto drive = unique_ptr<Drive>(new Drive(driveSettings, drivePins));

    auto axisPins = AxisPins(GPIO_NUM_13);
    auto axisSettings = AxisSettings(0.1f);
    return unique_ptr<Axis>(new Axis(move(drive), axisSettings, axisPins));
}

unique_ptr<Axis> setupYAxis()
{
    auto drivePins = DrivePins(GPIO_NUM_21, GPIO_NUM_22, GPIO_NUM_23);
    auto driveSettings = DriveSettings(800, 50, 3200);
    auto drive = unique_ptr<Drive>(new Drive(driveSettings, drivePins));

    auto axisPins = AxisPins(GPIO_NUM_25);
    auto axisSettings = AxisSettings(0.1f);
    return unique_ptr<Axis>(new Axis(move(drive), axisSettings, axisPins));
}

Application::Application() :
    xAxis(setupXAxis()),
    yAxis(setupYAxis()),
    displayProducer(shared_ptr<PositionChangedProducer>(new PositionChangedProducer(displayPositionChangedQueue, 30.0f))),
    serialProducer(shared_ptr<PositionChangedProducer>(new PositionChangedProducer(serialPositionChangedQueue, 1.0f)))
{
}

void Application::Handle(RequestBase request)
{
    // reinterpret cast
    // check if something can be done
    // set values, start processes
}

void Application::Tick()
{
    counter *= 1.00001;
    if (counter >= 1000)
    {
        counter = 1;
    }
 
    auto positionChanged = std::shared_ptr<PositionChangedMessage>(new PositionChangedMessage());

    positionChanged->payload.xAxis = counter;
    positionChanged->payload.yAxis = counter / 123;

    this->displayProducer->Produce(positionChanged);
    this->serialProducer->Produce(positionChanged);
}

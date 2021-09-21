#include <application.h>
#include <memory>

using namespace std;

int counter = 0;

struct WrappedMessage
{
    const unique_ptr<PositionChangedMessage> message;
    const shared_ptr<SerialReporter> serialReporter;
    const shared_ptr<DisplayReporter> displayReporter;

    WrappedMessage(
        unique_ptr<PositionChangedMessage> message,
        shared_ptr<SerialReporter> serialReporter,
        shared_ptr<DisplayReporter> displayReporter
    ) : 
        message(move(message)),
        serialReporter(serialReporter),
        displayReporter(displayReporter)
    {   
    }
};

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

Application::Application() : xAxis(setupXAxis()),
                             yAxis(setupYAxis()),
                             displayReporter(shared_ptr<DisplayReporter>(new DisplayReporter())),
                             serialReporter(shared_ptr<SerialReporter>(new SerialReporter()))
{
}

void Application::Handle(RequestBase request)
{
    // reinterpret cast
    // check if something can be done
    // set values, start processes
}

TaskHandle_t commyTask;

void Application::Tick()
{
    counter++;
    if (counter > 1001)
    {
        counter = -100;
    }

    delayMicroseconds(100);

    auto positionChanged = PositionChangedMessage();

    positionChanged.payload.xAxis = counter;
    positionChanged.payload.yAxis = counter / 123;

    this->displayReporter->Report(positionChanged);
    this->serialReporter->Report(positionChanged);

    // auto positionChanged2 = new PositionChangedMessage();

    // positionChanged2->payload.xAxis = counter;
    // positionChanged2->payload.yAxis = counter / 123;

    // if (commyTask == NULL)
    // {
    //     xTaskCreatePinnedToCore(
    //             Application::Report,
    //             "asd",
    //             256,
    //             positionChanged2,
    //             1,
    //             &commyTask,
    //             1);
    // }
    // else
    // {
    //     delete positionChanged2;
    // }
    
    // based on current variables, run AxisTick, run Drive Tick
    // Retrieve position
    // Schedule report function
}

void Application::Report(void* parameter)
{
    auto message = static_cast<WrappedMessage*>(parameter);
    
    message->displayReporter->Report(*(message->message));
    message->serialReporter->Report(*(message->message));

    delete message;

    vTaskDelete(NULL);
}

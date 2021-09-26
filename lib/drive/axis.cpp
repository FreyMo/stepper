#include <Arduino.h>
#include <axis.h>

AxisPins::AxisPins(uint8_t limitSwitchPin) : limitSwitch(limitSwitchPin) {}

Axis::Axis(
    std::unique_ptr<Drive> drive,
    const AxisPins pins) :
    drive(std::move(drive)),
    pins(pins)
{
    drive->Allow();
}

float counter = 1;

float Axis::Tick()
{
    counter *= 1.00002;
    if (counter >= 1000)
    {
        counter = 1;
    }

    return counter;
}

void Axis::Reference()
{
    if (!isRunning)
    {
        isRunning = true;

        // reference...

        isRunning = false;
    }
}

float Axis::DriveTo(float position)
{
    // NOT IMPLEMENTED
    return 0.0f;
}

float Axis::DriveFor(float distance)
{
    // NOT IMPLEMENTED
    drive->RotateByRevolutions(4.0, Direction::positive);
    delay(500);
    return drive->RotateByRevolutions(4.0, Direction::negative);
}

void Axis::Stop()
{
    drive->Disallow();
}

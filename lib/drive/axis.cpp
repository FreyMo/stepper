#include <Arduino.h>
#include <axis.h>

AxisPins::AxisPins(uint8_t limitSwitchPin) : limitSwitch(limitSwitchPin) {}

Axis::Axis(
    std::unique_ptr<Drive> drive,
    const AxisPins pins) :
    drive(std::move(drive)),
    pins(pins)
{
    position = 1;
}

float Axis::Tick()
{
    position *= 1.00002;
    if (position >= 1000)
    {
        position = 1;
    }

    return position;
}

bool Axis::Reference()
{
    if (state != AxisState::Standby)
    {
        return false;
    }

    state = AxisState::Referencing;

    // Save reference payload

    return true;
}

bool Axis::DriveTo(float position)
{
    if (state != AxisState::Standby)
    {
        return false;
    }

    state = AxisState::Driving;

    // Save drive to payload
    
    return true;
}

void Axis::Stop()
{
}

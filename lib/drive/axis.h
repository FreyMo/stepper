#pragma once

#include <memory>
#include <drive.h>

struct AxisSettings
{
    const float spindlePitch;

    AxisSettings(float spindlePitch);
};

struct AxisPins
{
    const uint8_t limitSwitch;

    AxisPins(uint8_t limitSwitchPin);
};

class Axis
{
private:
    const std::unique_ptr<Drive> drive;
    const AxisSettings settings;
    const AxisPins pins;
public:
    Axis(std::unique_ptr<Drive> drive, const AxisSettings settings, const AxisPins pins);

    void Reference();
    float DriveTo(float position);
    float DriveFor(float distance);
    void Stop();
};
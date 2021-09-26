#pragma once

#include <memory>
#include <drive.h>

struct AxisPins
{
    const uint8_t limitSwitch;

    AxisPins(uint8_t limitSwitchPin);
};

enum class AxisState
{
    Standby,
    Referencing,
    Driving
};

class Axis
{
    private:
        const std::unique_ptr<Drive> drive;
        const AxisPins pins;

        AxisState state = AxisState::Standby;
        float position = 0.0f;

    public:
        Axis(std::unique_ptr<Drive> drive, const AxisPins pins);

        float Tick();
        void Stop();

        bool Reference();
        bool DriveTo(float position);
};

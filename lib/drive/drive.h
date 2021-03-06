#pragma once

#include <memory>
#include <Arduino.h>

enum class Direction : uint8_t
{
    positive = HIGH,
    negative = LOW
};

struct DrivePins
{
    const uint8_t enable;
    const uint8_t direction;
    const uint8_t step;

    DrivePins(uint8_t enablePin, uint8_t directionPin, uint8_t stepPin);
};

// struct DriveMath
// {
//     static std::vector<uint32_t> GetStepDurations(const DriveSettings& settings, int steps);
// };

class Drive {
    private:
        const DrivePins pins;
        bool isAllowedToRun = false;

        void SetDirection(Direction direction);
        void SetStep(uint8_t value);
    public:
        Drive(const DrivePins pins);

        int RotateBySteps(int steps, Direction direction);
        float RotateByRevolutions(float revolutions, Direction direction);
        
        void Allow();
        void Disallow();
};

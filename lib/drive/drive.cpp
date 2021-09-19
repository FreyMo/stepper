#include <Arduino.h>
#include <drive.h>
#include <Wire.h>

DrivePins::DrivePins(uint8_t enablePin, uint8_t directionPin, uint8_t stepPin) :
    enable(enablePin), direction(directionPin), step(stepPin) {}

DriveSettings::DriveSettings(
    int stepsPerRevolution,
    int maxAcceleration,
    int maxVelocity) :
    stepsPerRevolution(stepsPerRevolution),
    maxAcceleration(maxAcceleration),
    maxVelocity(maxVelocity)
{
}

std::vector<uint32_t> DriveMath::GetStepDurations(const DriveSettings& settings, int steps)
{
    // constant velocity for now
    auto durationInMicroseconds = static_cast<uint32_t>(1000000.0f / static_cast<float>(settings.maxVelocity) / 2.0f);

    return std::vector<uint32_t>(steps, durationInMicroseconds);
    
    // auto accelerationSteps = static_cast<int>(std::powf(settings.velocity, 2.0f) / static_cast<float>(settings.acceleration) / 2.0f);
    // auto decelerationSteps = accelerationSteps;
    // auto stepsAtMaxVelocity = 0;

    // if (2 * accelerationSteps < steps)
    // {
    //     decelerationSteps = accelerationSteps;
    //     stepsAtMaxVelocity = steps - accelerationSteps - decelerationSteps;
    // }
    // else
    // {
    //     accelerationSteps = steps / 2;
    //     decelerationSteps = steps / 2;
    // }

    // return std::unique_ptr<std::vector<int>>();
}

Drive::Drive(const DriveSettings settings, const DrivePins pins) :
    settings(settings), pins(pins)
{
    pinMode(this->pins.enable, OUTPUT);
    pinMode(this->pins.direction, OUTPUT);
    pinMode(this->pins.step, OUTPUT);

    this->Disallow();
    this->SetDirection(Direction::positive);
    this->SetStep(LOW);
}

void Drive::SetDirection(Direction direction)
{
    digitalWrite(this->pins.direction, static_cast<uint8_t>(direction));
}

void Drive::SetStep(uint8_t value)
{
    digitalWrite(this->pins.step, value);
}

int Drive::RotateBySteps(int steps, Direction direction)
{
    auto stepDuration = DriveMath::GetStepDurations(this->settings, steps);
    auto performedSteps = 0;

    this->SetDirection(direction);

    for (auto it = stepDuration.begin(); it != stepDuration.end(); ++it)
    {
        if (!this->isAllowedToRun)
        {
            return performedSteps;
        }

        digitalWrite(this->pins.enable, LOW);
    
        this->SetStep(HIGH);
        delayMicroseconds(*it);
        
        this->SetStep(LOW);
        delayMicroseconds(*it);

        performedSteps++;
    }

    return performedSteps;
}

float Drive::RotateByRevolutions(float revolutions, Direction direction)
{
    auto performedSteps = this->RotateBySteps(this->settings.stepsPerRevolution * revolutions, direction);
    auto performedRevolutions = (static_cast<float>(performedSteps) / static_cast<float>(this->settings.stepsPerRevolution));

    return performedRevolutions;
}

void Drive::Allow()
{
    this->isAllowedToRun = true;
}

void Drive::Disallow()
{
    digitalWrite(this->pins.enable, HIGH);
    this->isAllowedToRun = false;
}

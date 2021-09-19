#include <Arduino.h>
#include <axis.h>

AxisSettings::AxisSettings(float spindlePitch) : spindlePitch(spindlePitch) {}

AxisPins::AxisPins(uint8_t limitSwitchPin) : limitSwitch(limitSwitchPin) {}

Axis::Axis(
    std::unique_ptr<Drive> drive,
    const AxisSettings settings,
    const AxisPins pins) :
    drive(std::move(drive)),
    settings(settings),
    pins(pins)
{
    this->drive->Allow();
}

void Axis::Reference()
{
    if (!this->isRunning)
    {
        this->isRunning = true;

        // reference...

        this->isRunning = false;
    }
}

void Axis::ReferenceAsync()
{
    xTaskCreatePinnedToCore(
        Axis::ReferenceStatic,
        "Reference",
        5000,
        this,
        1,
        NULL,
        0 // SHOULD BE SET AT START
    );
}

void Axis::ReferenceStatic(void* parameter)
{
    auto axis = static_cast<Axis*>(parameter);
    axis->Reference();
    vTaskDelete(NULL);
}

float Axis::DriveTo(float position)
{
    // NOT IMPLEMENTED
    return 0.0f;
}

float Axis::DriveFor(float distance)
{
    // NOT IMPLEMENTED
    this->drive->RotateByRevolutions(4.0, Direction::positive);
    delay(500);
    return this->drive->RotateByRevolutions(4.0, Direction::negative);
}

void Axis::Stop()
{
    this->drive->Disallow();
}

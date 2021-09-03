#include <Arduino.h>
#include <drive.h>
#include <axis.h>

#include <communication.h>

std::unique_ptr<Axis> axis;
std::unique_ptr<Communication> communication;

IRAM_ATTR void interrupt() {
  axis->Stop();
}

void limitAxis(const AxisPins& pins)
{
  pinMode(pins.limitSwitch, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pins.limitSwitch), interrupt, RISING);
}

void setup()
{
  Serial.setTimeout(5); // default is 1000 which slows down the communication drastically
  Serial.begin(115200);
  Serial.println("Started up");
  
  communication = std::unique_ptr<Communication>();
  
  auto drive = std::unique_ptr<Drive>(new Drive(DriveSettings(800, 50, 3200), DrivePins(GPIO_NUM_16, GPIO_NUM_4, GPIO_NUM_17)));

  auto axisPins = AxisPins(GPIO_NUM_13);
  axis = std::unique_ptr<Axis>(new Axis(std::move(drive), AxisSettings(0.1f), axisPins));
  limitAxis(axisPins);
}

void loop()
{
  if (Serial.available() > 0)
  {
    auto request = communication->Receive(Serial);
    if (request.hasValue)
    {
      communication->Transmit(Response(request.value.id, request.value.action), Serial);
    }
    else
    {
      communication->Transmit(Response("NONE", "failure"), Serial);
    }
  }

  // axis->DriveFor(1.0);
  // delay(500);
}
#include <Arduino.h>
#include <drive.h>
#include <axis.h>
#include <display.h>
#include <display_reporter.h>
#include <serial_reporter.h>

#include <communication.h>

std::unique_ptr<Axis> xAxis;
std::unique_ptr<Axis> yAxis;
std::unique_ptr<Communication> communication;
std::unique_ptr<DisplayReporter> displayReporter;
std::unique_ptr<SerialReporter> serialReporter;

// IRAM_ATTR void interrupt() {
//   axis->Stop();
// }

// void limitAxis(const AxisPins& pins)
// {
//   pinMode(pins.limitSwitch, INPUT_PULLUP);
//   attachInterrupt(digitalPinToInterrupt(pins.limitSwitch), interrupt, RISING);
// }

void setupSerial()
{
  Serial.setTimeout(5); // default is 1000 which slows down the communication drastically
  Serial.begin(115200);
  Serial.println("Started up");

  communication = std::unique_ptr<Communication>();
}

void setupXAxis()
{
  auto drivePins = DrivePins(GPIO_NUM_16, GPIO_NUM_4, GPIO_NUM_17);
  auto driveSettings = DriveSettings(800, 50, 3200);
  auto drive = std::unique_ptr<Drive>(new Drive(driveSettings, drivePins));

  auto axisPins = AxisPins(GPIO_NUM_13);
  auto axisSettings = AxisSettings(0.1f);
  xAxis = std::unique_ptr<Axis>(new Axis(std::move(drive), axisSettings, axisPins));
}

void setupYAxis()
{
  auto drivePins = DrivePins(GPIO_NUM_21, GPIO_NUM_22, GPIO_NUM_23);
  auto driveSettings = DriveSettings(800, 50, 3200);
  auto drive = std::unique_ptr<Drive>(new Drive(driveSettings, drivePins));

  auto axisPins = AxisPins(GPIO_NUM_25);
  auto axisSettings = AxisSettings(0.1f);
  xAxis = std::unique_ptr<Axis>(new Axis(std::move(drive), axisSettings, axisPins));
}

void setup()
{
  setupSerial();
  setupXAxis();
  setupYAxis();

  delay(50);
  displayReporter = std::unique_ptr<DisplayReporter>(new DisplayReporter(10.0F));
  delay(50);
  serialReporter = std::unique_ptr<SerialReporter>(new SerialReporter(2.0F));
}

float counter = -150.7;

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

  auto start = micros();

  counter++;
  if (counter > 1400) { counter -= 1512.3; }
  displayReporter->Report(counter);
  auto status = Status("axisY", counter);
  serialReporter->Report(status);

  auto stop = micros();

  Serial.println(stop - start);
}
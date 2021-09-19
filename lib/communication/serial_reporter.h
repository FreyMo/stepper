#pragma once

#include <reporter.h>
#include <ArduinoJson.h>

struct Status
{
    const String name;
    const float value;

    Status(const String name, float value) : name(name), value(value) {}
};

class SerialReporter : public Reporter<Status&>
{
public:
    SerialReporter(float frequencyInHertz) : Reporter<Status&>(frequencyInHertz) {}
protected:
    virtual void ReportInternal(Status& value) override;
};

void SerialReporter::ReportInternal(Status& value)
{
    StaticJsonDocument<64> json;

    json["name"] = value.name;
    json["value"] = value.value;
    
    serializeJson(json, Serial);
    Serial.println();
}

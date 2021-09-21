#include <serial_reporter.h>
#include <definitions/message.h>
#include <ArduinoJson.h>

SerialReporter::SerialReporter(float frequencyInHertz) :
    Reporter<const PositionChangedMessage&>(frequencyInHertz)
{
}

void SerialReporter::ReportInternal(const PositionChangedMessage& value)
{
    StaticJsonDocument<128> doc;
    auto json = doc.to<JsonObject>();

    json["type"] = MessageBase::type;
    json["event"] = PositionChangedMessage::eventString;

    auto payload = json.createNestedObject("payload");
    
    payload["xAxis"] = value.payload.xAxis;
    payload["yAxis"] = value.payload.yAxis;

    serializeJson(json, Serial);
    Serial.println();
}

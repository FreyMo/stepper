#pragma once

#include <definitions/message.h>
#include <display.h>
#include <message_queues.h>
#include <ArduinoJson.h>
#include <memory>

class QueueProcessor
{
    private:
        std::unique_ptr<Display> display;

        void HandleSerialMessage(std::shared_ptr<PositionChangedMessage> message);
        void HandleDisplayMessage(std::shared_ptr<PositionChangedMessage> message);
    
    public:
        void ProcessMessageQueues();

        QueueProcessor() : display(std::unique_ptr<Display>(new Display(0x70)))
        {
        }
};

void QueueProcessor::HandleSerialMessage(std::shared_ptr<PositionChangedMessage> message)
{
    StaticJsonDocument<128> doc;
    auto json = doc.to<JsonObject>();

    json["type"] = MessageBase::type;
    json["event"] = PositionChangedMessage::eventString;

    auto payload = json.createNestedObject("payload");
    
    payload["xAxis"] = message->payload.xAxis;
    payload["yAxis"] = message->payload.yAxis;

    serializeJson(json, Serial);
    Serial.println();

    vTaskDelay(1);
}

void QueueProcessor::HandleDisplayMessage(std::shared_ptr<PositionChangedMessage> message)
{
    auto start = micros();
    this->display->Show(message->payload.xAxis);
    auto stopped = micros();

    if (stopped - start > 10000)
    {
        Serial.println("Took too long! " + String(stopped - start));
    } 
}

void QueueProcessor::ProcessMessageQueues() {
    if (!serialPositionChangedQueue.empty())
    {
        auto serialMessage = serialPositionChangedQueue.front();
        serialPositionChangedQueue.pop();
        this->HandleSerialMessage(serialMessage);
    }

    if (!displayPositionChangedQueue.empty())
    {
        auto displayMessage = displayPositionChangedQueue.front();
        displayPositionChangedQueue.pop();
        this->HandleDisplayMessage(displayMessage);
    }
}

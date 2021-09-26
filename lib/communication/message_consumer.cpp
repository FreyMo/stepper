#include <message_consumer.h>

void MessageConsumer::HandleSerialMessage(std::shared_ptr<PositionChangedMessage> message)
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
}

void MessageConsumer::HandleErrorMessage(std::shared_ptr<ErrorOccurredMessage> message)
{
    StaticJsonDocument<128> doc;
    auto json = doc.to<JsonObject>();

    json["type"] = MessageBase::type;
    json["event"] = ErrorOccurredMessage::eventString;

    auto payload = json.createNestedObject("payload");
    
    payload["error"] = message->payload.error;

    serializeJson(json, Serial);
    Serial.println();
}

void MessageConsumer::HandleDisplayMessage(std::shared_ptr<PositionChangedMessage> message)
{
    display->Show(message->payload.xAxis);
}

void MessageConsumer::Consume()
{
    if (!serialPositionChangedQueue->empty())
    {
        auto serialMessage = serialPositionChangedQueue->front();
        serialPositionChangedQueue->pop();
        HandleSerialMessage(serialMessage);
    }

    if (!displayPositionChangedQueue->empty())
    {
        auto displayMessage = displayPositionChangedQueue->front();
        displayPositionChangedQueue->pop();
        HandleDisplayMessage(displayMessage);
    }

    if (!errorOccuredQueue->empty())
    {
        auto errorMessage = errorOccuredQueue->front();
        errorOccuredQueue->pop();
        HandleErrorMessage(errorMessage);
    }
}

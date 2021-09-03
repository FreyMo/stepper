#include <ArduinoJson.h>
#include <communication.h>

Optional<Request> Communication::Receive(Stream& stream)
{
    DynamicJsonDocument json(this->size);

    auto error = deserializeJson(json, stream);
    if (error.code() != ArduinoJson::DeserializationError::Code::Ok)
    {
        return Optional<Request>::None();
    }

    // Necessary to prevent read of only \n
    auto _ = stream.readString();

    auto request = Request();
    request.id      = json["id"].as<String>();
    request.action  = json["action"].as<String>();

    return Optional<Request>::Some(request);
}

void Communication::Transmit(const Response& response, Stream& stream)
{
    DynamicJsonDocument json(this->size);

    json["id"] = response.id;
    json["status"] = response.status;
    
    serializeJson(json, stream);
    stream.println();
}

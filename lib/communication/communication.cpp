#include <ArduinoJson.h>
#include <communication.h>

ReferenceRequest ParseReferenceRequest(const StaticJsonDocument<512>& json)
{
    auto request = ReferenceRequest();

    request.id     = json["id"].as<String>();
    request.action = json["action"].as<String>();

    request.payload.referenceSpeed = json["payload"]["referenceSpeed"].as<float>();
    request.payload.direction      = json["payload"]["direction"].as<String>();

    return request;
}

DriveToRequest ParseDriveToRequest(const StaticJsonDocument<512>& json)
{
    auto request = DriveToRequest();
    
    request.id     = json["id"].as<String>();
    request.action = json["action"].as<String>();

    request.payload.axisX.position     = json["payload"]["axisX"]["position"].as<float>();
    request.payload.axisX.velocity     = json["payload"]["axisX"]["velocity"].as<float>();
    request.payload.axisX.acceleration = json["payload"]["axisX"]["acceleration"].as<float>();

    request.payload.axisY.position     = json["payload"]["axisY"]["position"].as<float>();
    request.payload.axisY.velocity     = json["payload"]["axisY"]["velocity"].as<float>();
    request.payload.axisY.acceleration = json["payload"]["axisY"]["acceleration"].as<float>();

    return request;
}

Optional<RequestBase> Communication::Receive(Stream& stream)
{
    StaticJsonDocument<512> json;
    
    auto error = deserializeJson(json, stream);
    if (error.code() != ArduinoJson::DeserializationError::Code::Ok)
    {
        return Optional<RequestBase>::None();
    }

    auto _ = stream.readString();
    auto action = json["action"].as<String>();

    if (action == ReferenceRequest::actionString)
    {
        return Optional<RequestBase>::Some(ParseReferenceRequest(json));
    }
    else if (action == DriveToRequest::actionString)
    {
        return Optional<RequestBase>::Some(ParseDriveToRequest(json));
    }

    return Optional<RequestBase>::None();
}

void Communication::Transmit(const Response& response, Stream& stream)
{
    StaticJsonDocument<128> json;

    json["type"] = Response::type;
    json["id"] = response.id;
    json["status"] = response.status;
    
    serializeJson(json, stream);
    stream.println();
}

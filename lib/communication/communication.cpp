#include <ArduinoJson.h>
#include <communication.h>

ReferenceRequest ParseReferenceRequest(DynamicJsonDocument& json)
{
    auto request = ReferenceRequest();
    request.id     = json["id"].as<String>();
    request.action = json["action"].as<String>();

    // request.payload = ReferencePayload();

    request.payload.referenceSpeed = json["payload"]["referenceSpeed"].as<float>();
    request.payload.direction      = json["payload"]["direction"].as<String>();

    return request;
}

DriveToRequest ParseDriveToRequest(DynamicJsonDocument& json)
{
    auto request = DriveToRequest();
    request.id     = json["id"].as<String>();
    request.action = json["action"].as<String>();

    // request.payload = DriveToPayload();
    // request.payload.axisX = DriveToAxis();
    // request.payload.axisY = DriveToAxis();
    
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
    DynamicJsonDocument json(this->size);

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
    DynamicJsonDocument json(this->size);

    json["type"] = Response::type;
    json["id"] = response.id;
    json["status"] = response.status;
    
    serializeJson(json, stream);
    stream.println();
}

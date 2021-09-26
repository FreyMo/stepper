#include <ArduinoJson.h>
#include <communication.h>

using namespace std;

typedef StaticJsonDocument<512> RequestJsonDocument;
typedef StaticJsonDocument<128> ResponseJsonDocument;

Communication::Communication()
{
    Serial.setTimeout(5);
    Serial.begin(115200);
}

shared_ptr<ReferenceRequest> ParseReferenceRequest(const RequestJsonDocument& json)
{
    auto request = shared_ptr<ReferenceRequest>(new ReferenceRequest());

    request->id     = json["id"].as<String>();
    request->action = json["action"].as<String>();

    request->payload.axis         = json["payload"]["axis"].as<String>();
    request->payload.direction    = json["payload"]["direction"].as<String>();
    request->payload.velocity     = json["payload"]["velocity"].as<float>();
    request->payload.acceleration = json["payload"]["acceleration"].as<float>();

    request->payload.spindlePitch       = json["payload"]["spindlePitch"].as<float>();
    request->payload.stepsPerRevolution = json["payload"]["stepsPerRevolution"].as<uint16_t>();
    request->payload.upperLimit         = json["payload"]["upperLimit"].as<float>();
    request->payload.lowerLimit         = json["payload"]["lowerLimit"].as<float>();
    request->payload.referenceAt        = json["payload"]["referenceAt"].as<float>();

    return request;
}

shared_ptr<DriveToRequest> ParseDriveToRequest(const RequestJsonDocument& json)
{
    auto request = shared_ptr<DriveToRequest>(new DriveToRequest());
    
    request->id     = json["id"].as<String>();
    request->action = json["action"].as<String>();

    request->payload.axisX.position     = json["payload"]["axisX"]["position"].as<float>();
    request->payload.axisX.velocity     = json["payload"]["axisX"]["velocity"].as<float>();
    request->payload.axisX.acceleration = json["payload"]["axisX"]["acceleration"].as<float>();

    request->payload.axisY.position     = json["payload"]["axisY"]["position"].as<float>();
    request->payload.axisY.velocity     = json["payload"]["axisY"]["velocity"].as<float>();
    request->payload.axisY.acceleration = json["payload"]["axisY"]["acceleration"].as<float>();

    return request;
}

Optional<RequestBase> Communication::Receive(Stream& stream)
{
    RequestJsonDocument json;
    
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
    ResponseJsonDocument json;

    json["type"]   = Response::type;
    json["id"]     = response.id;
    json["status"] = response.status;
    
    serializeJson(json, stream);
    stream.println();
}

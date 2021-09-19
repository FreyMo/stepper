#pragma once
#include <Arduino.h>

struct RequestBase {
    String id;
    String action;
};

template <typename T>
struct Request : RequestBase
{
    T payload;
};

struct ReferencePayload
{
    float referenceSpeed;
    String direction;
};

struct ReferenceRequest : public Request<ReferencePayload>
{
    static const String actionString;
};

const String ReferenceRequest::actionString = "reference";

struct DriveToAxis
{
    float position;
    float velocity;
    float acceleration;
};

struct DriveToPayload
{
    DriveToAxis axisX;
    DriveToAxis axisY;
};

struct DriveToRequest : public Request<DriveToPayload>
{
    static const String actionString;
};

const String DriveToRequest::actionString = "driveTo";

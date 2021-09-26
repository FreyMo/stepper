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
    String axis;
    String direction;
    float velocity;
    float acceleration;
    float spindlePitch;
    uint16_t stepsPerRevolution;
    float upperLimit;
    float lowerLimit;
    float referenceAt;
};

struct ReferenceRequest : public Request<ReferencePayload>
{
    static const String actionString;
};

struct DriveToPayload
{
    String axis;
    float position;
    float velocity;
    float acceleration;
};

struct DriveToRequest : public Request<DriveToPayload>
{
    static const String actionString;
};

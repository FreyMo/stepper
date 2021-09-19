#pragma once
#include <Arduino.h>

struct MessageBase
{
    static const String type;
};

template <typename T>
struct Message : public MessageBase
{
};

struct PositionChangedPayload
{
    float axisX;
    float axisY;
};

struct PositionChangedMessage : public Message<PositionChangedPayload>
{
};

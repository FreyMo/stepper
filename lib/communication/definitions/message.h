#pragma once
#include <Arduino.h>

template <typename T>
struct Message
{
};

struct PositionChangedPayload
{
    String axisX;
    String axisY;
};

struct PositionChangedMessage : public Message<PositionChangedPayload>
{
};
#pragma once

#include <optional.h>
#include <Arduino.h>
#include <definitions/request.h>
#include <definitions/response.h>

class Communication
{
public:
    Communication();
    Optional<RequestBase> Receive(Stream& stream);
    void Transmit(const Response& response, Stream& stream);
};

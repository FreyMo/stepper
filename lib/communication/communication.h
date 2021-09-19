#pragma once

#include <optional.h>
#include <Arduino.h>
#include <definitions/request.h>
#include <definitions/response.h>

class Communication
{
private:
    const size_t size = 1024;
public:
    Optional<RequestBase> Receive(Stream& stream);
    void Transmit(const Response& response, Stream& stream);
};

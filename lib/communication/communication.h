#pragma once

#include <optional.h>
#include <Arduino.h>

struct Request {
    String id;
    String action;
};

struct Response {
    const String id;
    const String status;

    Response(String id, String status) : id(id), status(status) {}
};

class Communication
{
private:
    const size_t size = 1024;
public:
    Optional<Request> Receive(Stream& stream);
    void Transmit(const Response& response, Stream& stream);
};

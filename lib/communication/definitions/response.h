#pragma once
#include <Arduino.h>

enum class ResponseStatus
{
    ok,
    error
};

struct Response {
    const String id;
    const ResponseStatus status;
    static const String type;

    Response(String id, ResponseStatus status) : id(id), status(status) {}
};

const String Response::type = "response";
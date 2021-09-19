#pragma once
#include <Arduino.h>

enum class ResponseStatus
{
    ok,
    error
};

struct Response {
    const String id;
    const String status;
    static const String type;

    Response(String id, String status) : id(id), status(status) {}
};

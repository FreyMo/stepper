#pragma once
#include <Arduino.h>

struct ResponseStatus
{
    static const String ok;
    static const String error;
};

struct Response {
    const String id;
    const String status;
    static const String type;

    Response(String id, String status) : id(id), status(status) {}
};

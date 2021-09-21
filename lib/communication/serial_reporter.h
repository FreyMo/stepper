#pragma once

#include <reporter.h>
#include <definitions/message.h>

class SerialReporter : public Reporter<const PositionChangedMessage&>
{
public:
    SerialReporter(float frequencyInHertz = 1.0f);
protected:
    virtual void ReportInternal(const PositionChangedMessage& value) override;
};

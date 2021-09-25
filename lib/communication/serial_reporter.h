#pragma once

#include <reporter.h>
#include <definitions/message.h>

class SerialReporter : public Reporter<PositionChangedMessage>
{
public:
    SerialReporter(float frequencyInHertz = 1.0f);
protected:
    virtual void ReportInternal(std::shared_ptr<PositionChangedMessage> value) override;
};

#pragma once

#include <reporter.h>
#include <display.h>
#include <definitions/message.h>
#include <memory>

class DisplayReporter : public Reporter<const PositionChangedMessage&>
{
private:
    const std::unique_ptr<Display> xAxisDisplay;
    const std::unique_ptr<Display> yAxisDisplay;

public:
    DisplayReporter(float frequencyInHertz = 30.0f);

protected:
    virtual void ReportInternal(const PositionChangedMessage& value) override;
};

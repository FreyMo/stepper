#pragma once

#include <reporter.h>
#include <display.h>
#include <definitions/message.h>

class DisplayReporter : public Reporter<const PositionChangedMessage&>
{
private:
    Display xAxisDisplay;
    Display yAxisDisplay;

public:
    DisplayReporter(float frequencyInHertz);

protected:
    virtual void ReportInternal(const PositionChangedMessage& value) override;
};

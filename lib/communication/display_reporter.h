#pragma once

#include <reporter.h>
#include <display.h>

class DisplayReporter : public Reporter<float>
{
private:
    Display display;

public:
    DisplayReporter(float frequencyInHertz) : Reporter<float>(frequencyInHertz), display(Display()) {}
protected:
    virtual void ReportInternal(float value) override;
};

void DisplayReporter::ReportInternal(float value)
{
    display.Show(value);
}
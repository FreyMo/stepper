#include <display_reporter.h>
#include <display.h>

DisplayReporter::DisplayReporter(float frequencyInHertz) :
    Reporter<const PositionChangedMessage&>(frequencyInHertz),
    xAxisDisplay(Display(0x70)),
    yAxisDisplay(Display(0x71))
{
}

void DisplayReporter::ReportInternal(const PositionChangedMessage& value)
{
    this->xAxisDisplay.Show(value.payload.axisX);
    this->yAxisDisplay.Show(value.payload.axisY);
}

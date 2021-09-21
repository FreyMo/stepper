#include <display_reporter.h>
#include <display.h>

DisplayReporter::DisplayReporter(float frequencyInHertz) :
    Reporter<const PositionChangedMessage&>(frequencyInHertz),
    xAxisDisplay(std::unique_ptr<Display>(new Display(0x70))),
    yAxisDisplay(std::unique_ptr<Display>(new Display(0x71)))
{
}

void DisplayReporter::ReportInternal(const PositionChangedMessage& value)
{
    this->xAxisDisplay->Show(value.payload.xAxis);
    this->yAxisDisplay->Show(value.payload.yAxis);
}

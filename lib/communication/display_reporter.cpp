#include <display_reporter.h>
#include <display.h>
#include <message_queues.h>

DisplayReporter::DisplayReporter(float frequencyInHertz) :
    Reporter<PositionChangedMessage>(frequencyInHertz),
    xAxisDisplay(std::unique_ptr<Display>(new Display(0x77))), // MUST BE 0x70
    yAxisDisplay(std::unique_ptr<Display>(new Display(0x71)))
{
}

void DisplayReporter::ReportInternal(std::shared_ptr<PositionChangedMessage> value)
{
    if (displayPositionChangedQueue.size() < 10)
    {
        displayPositionChangedQueue.push(value);
    }
}

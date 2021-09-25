#include <serial_reporter.h>
#include <definitions/message.h>
#include <ArduinoJson.h>
#include <message_queues.h>

SerialReporter::SerialReporter(float frequencyInHertz) :
    Reporter<PositionChangedMessage>(frequencyInHertz)
{
}

void SerialReporter::ReportInternal(std::shared_ptr<PositionChangedMessage> value)
{
    if (serialPositionChangedQueue.size() < 3)
    {
        serialPositionChangedQueue.push(value);
    }
}

#pragma once

#include <message_producer.h>
#include <definitions/message.h>
#include <memory>

using namespace std;

class ErrorOccuredProducer : public MessageProducer<ErrorOccurredMessage>
{
    public:
        ErrorOccuredProducer(
            shared_ptr<queue<shared_ptr<ErrorOccurredMessage>>> messages,
            float frequencyInHertz) :
            MessageProducer<ErrorOccurredMessage>(messages, frequencyInHertz)
            {
            }
};

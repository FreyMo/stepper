#pragma once

#include <message_producer.h>
#include <definitions/message.h>
#include <memory>

using namespace std;

class PositionChangedProducer : public MessageProducer<PositionChangedMessage>
{
    public:
        PositionChangedProducer(
            shared_ptr<queue<shared_ptr<PositionChangedMessage>>> messages,
            float frequencyInHertz) :
            MessageProducer<PositionChangedMessage>(messages, frequencyInHertz)
            {
            }
};

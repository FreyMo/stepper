#pragma once
#pragma once

#include <definitions/message.h>
#include <display.h>
#include <message_queues.h>
#include <ArduinoJson.h>
#include <memory>

class MessageConsumer
{
    private:
        std::unique_ptr<Display> display;

        void HandleSerialMessage(std::shared_ptr<PositionChangedMessage> message);
        void HandleDisplayMessage(std::shared_ptr<PositionChangedMessage> message);
    
    public:
        void Consume();

        MessageConsumer() : display(std::unique_ptr<Display>(new Display(0x70)))
        {
        }
};

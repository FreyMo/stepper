#include <Arduino.h>
#include <communication.h>
#include <application.h>
#include <memory>
#include <message_consumer.h>

#define HIGH_PRIORITY 3
#define LOW_PRIORITY 2
#define APPLICATION_CORE 1
#define COMMUNICATION_CORE 0

using namespace std;

unique_ptr<Communication> communication;
unique_ptr<Application> application;
unique_ptr<MessageConsumer> messageConsumer;

TaskHandle_t communicationTask;
TaskHandle_t applicationTask;
TaskHandle_t messageTask;

void RunApplication(void* parameters)
{
    auto app = static_cast<Application*>(parameters);

    while (true)
    {
        auto started = micros();
        app->Tick();
        auto stopped = micros();

        if (stopped - started > 70)
        {
            Serial.println("Tick took too long! Alert!");
        }
    }
}

void RunCommunication(void* parameters)
{
    auto comm = static_cast<Communication*>(parameters);

    while (true)
    {
        if (Serial.available() > 0)
        {
            auto request = comm->Receive(Serial);
            if (request.hasValue)
            {
                auto response = application->Handle(request.value);
                comm->Transmit(*response.get(), Serial);
            }
            else
            {
                comm->Transmit(Response("", ResponseStatus::error), Serial);
            }
        }

        vTaskDelay(1);
    }
}

void ProcessMessageQueues(void* parameters)
{
    while (true)
    {
        messageConsumer->Consume();

        vTaskDelay(1);
    }
}

void setup()
{
    communication = unique_ptr<Communication>(new Communication());
    application = unique_ptr<Application>(new Application());
    messageConsumer = unique_ptr<MessageConsumer>(new MessageConsumer());

    disableCore0WDT();
    disableCore1WDT();

    // Do not change the order of these tasks!
    xTaskCreatePinnedToCore(
        RunCommunication,
        "RunCommunication",
        50000,
        communication.get(),
        LOW_PRIORITY,
        &communicationTask,
        COMMUNICATION_CORE
    );

    xTaskCreatePinnedToCore(
        ProcessMessageQueues,
        "ProcessMessageQueues",
        50000,
        NULL,
        LOW_PRIORITY,
        &messageTask,
        COMMUNICATION_CORE
    );

    xTaskCreatePinnedToCore(
        RunApplication,
        "RunApplication",
        50000,
        application.get(),
        HIGH_PRIORITY,
        &applicationTask,
        APPLICATION_CORE);
}

void loop()
{
    vTaskDelete(NULL);
}

#include <Arduino.h>
#include <communication.h>
#include <application.h>
#include <memory>

#define HIGH_PRIORITY 1
#define LOW_PRIORITY 0
#define APPLICATION_CORE 0
#define COMMUNICATION_CORE 1

std::unique_ptr<Communication> communication;
std::unique_ptr<Application> application;

TaskHandle_t communicationTask;
TaskHandle_t applicationTask;

void RunApplication(void* parameters) {
    auto app = static_cast<Application*>(parameters);

    while (true)
    {
        app->Tick();

        vTaskDelay(1);
    }
}

void RunCommunication(void* parameters) {
    auto comm = static_cast<Communication*>(parameters);

    while (true)
    {
        if (Serial.available() > 0)
        {
            auto request = comm->Receive(Serial);
            if (request.hasValue)
            {
                comm->Transmit(Response(request.value.id, "ok"), Serial);
                application->Handle(request.value);
            }
            else
            {
                comm->Transmit(Response("", "error"), Serial);
            }
        }

        vTaskDelay(1);
    }
}

void setup()
{
    communication = std::unique_ptr<Communication>(new Communication());
    application = std::unique_ptr<Application>(new Application());

    disableCore0WDT();
    disableCore1WDT();

    vTaskPrioritySet(NULL, LOW_PRIORITY);

    xTaskCreatePinnedToCore(
        RunApplication,
        "RunApplication",
        10000,
        application.get(),
        HIGH_PRIORITY,
        &applicationTask,
        APPLICATION_CORE);

    xTaskCreatePinnedToCore(
        RunCommunication,
        "RunCommunication",
        50000,
        communication.get(),
        HIGH_PRIORITY,
        &communicationTask,
        COMMUNICATION_CORE
    );
}

void loop()
{
    vTaskDelay(1);
}

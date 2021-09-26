#pragma once

#include <Arduino.h>
#include <memory>
#include <queue>

using namespace std;

template <typename T>
class MessageProducer
{
    private:
        shared_ptr<queue<shared_ptr<T>>> messages;
        const float frequencyInHertz;
        const float cycleDurationInMicros;
        unsigned long microsAtStart;

    public:
        MessageProducer(shared_ptr<queue<shared_ptr<T>>> messages, float frequencyInHertz);
        void Produce(shared_ptr<T> value, bool force = false);
};

template <typename T>
MessageProducer<T>::MessageProducer(shared_ptr<queue<shared_ptr<T>>> messages, float frequencyInHertz) :
    messages(messages),
    frequencyInHertz(frequencyInHertz),
    cycleDurationInMicros(1000000.0f / frequencyInHertz),
    microsAtStart(micros())
{
}

template <typename T>
void MessageProducer<T>::Produce(std::shared_ptr<T> value, bool force)
{
    if (force)
    {
        messages->push(value);

        return;
    }

    auto elapsedTicks = static_cast<unsigned long>((micros() - microsAtStart) / cycleDurationInMicros);

    if (elapsedTicks >= 1)
    {
        microsAtStart += (elapsedTicks * cycleDurationInMicros);

        if (messages->size() < 3)
        {
            messages->push(value);
        }
    }
}

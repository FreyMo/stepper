#pragma once

#include <Arduino.h>
#include <memory>
#include <queue>

using namespace std;

template <typename T>
class MessageProducer
{
    private:
        queue<shared_ptr<T>>* messages;
        const float frequencyInHertz;
        const float cycleDurationInMicros;
        unsigned long microsAtStart;

    public:
        MessageProducer(queue<shared_ptr<T>>* messages, float frequencyInHertz);
        void Produce(shared_ptr<T> value, bool force = false);
};

template <typename T>
MessageProducer<T>::MessageProducer(queue<shared_ptr<T>>* messages, float frequencyInHertz) :
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
        this->messages->push(value);

        return;
    }

    auto elapsedTicks = (micros() - this->microsAtStart) / this->cycleDurationInMicros;

    if (elapsedTicks >= 1)
    {
        this->microsAtStart += elapsedTicks * this->cycleDurationInMicros;

        if (this->messages->size() < 3)
        {
            this->messages->push(value);
        }
    }
}

#pragma once

#include <Arduino.h>

template <typename T>
class Reporter
{    
private:
    const float frequencyInHertz;
    const float microsPerReport;
    const unsigned long microsAtStart;

    unsigned long timesReported;

protected:
    virtual void ReportInternal(T value) = 0;

public:
    Reporter(float frequencyInHertz = 30.0F);
    void Report(T value);
};

template <typename T>
Reporter<T>::Reporter(float frequencyInHertz) :
    frequencyInHertz(frequencyInHertz),
    microsPerReport(1000000.0F / frequencyInHertz),
    microsAtStart(micros()),
    timesReported(0)
{
}

template <typename T>
void Reporter<T>::Report(T value)
{
  auto elapsedMicros = micros() - this->microsAtStart;

  if (elapsedMicros >= this->timesReported * this->microsPerReport)
  {
    this->timesReported++;
    
    this->ReportInternal(value);
  }
}

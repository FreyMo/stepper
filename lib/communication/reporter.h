#pragma once

#include <Arduino.h>
#include <memory>

template <typename T>
class Reporter
{
private:
    const float frequencyInHertz;
    const float microsPerReport;
    unsigned long microsAtStart;

protected:
    virtual void ReportInternal(std::shared_ptr<T> value) = 0;

public:
    Reporter(float frequencyInHertz = 30.0F);
    void Report(std::shared_ptr<T> value, bool force = false);
};

template <typename T>
Reporter<T>::Reporter(float frequencyInHertz) :
    frequencyInHertz(frequencyInHertz),
    microsPerReport(1000000.0f / frequencyInHertz),
    microsAtStart(micros())
{
}

template <typename T>
void Reporter<T>::Report(std::shared_ptr<T> value, bool force)
{
  if (force)
  {
    this->ReportInternal(value);

    return;
  }

  auto elapsedReports = (micros() - this->microsAtStart) / this->microsPerReport;

  if (elapsedReports >= 1)
  {
    this->microsAtStart += elapsedReports * this->microsPerReport;

    this->ReportInternal(value);
  }
}

#pragma once

#include <memory>

template <typename T>
struct Optional
{
    const std::shared_ptr<T> value;
    const bool hasValue;

    static Optional<T> Some(std::shared_ptr<T> value) { return Optional(value); }
    static Optional<T> None() { return Optional(); };
    
private:
    Optional(std::shared_ptr<T> value) : value(value), hasValue(true) {}
    Optional() : hasValue(false) {}
};

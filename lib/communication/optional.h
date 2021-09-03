#pragma once

template <typename T>
struct Optional
{
    const T value;
    const bool hasValue;

    static Optional<T> Some(T value) { return Optional(value); }
    static Optional<T> None() { return Optional(); };
    
private:
    Optional(T value) : value(value), hasValue(true) {}
    Optional() : hasValue(false) {}
};

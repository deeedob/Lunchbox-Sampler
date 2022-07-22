#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <define_t40.hpp>

class LunchboxSampler
{
public:
    LunchboxSampler(const LunchboxSampler&) = delete;
    LunchboxSampler& operator=(const LunchboxSampler& other) = delete;

    static LunchboxSampler& getInstance();
    void run();
private:
    LunchboxSampler();
    ~LunchboxSampler();

    void setup();
};
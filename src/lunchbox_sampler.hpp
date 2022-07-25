#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <define_t40.hpp>
#include <event_sytem.hpp>

using namespace lbs;

class LunchboxSampler
{
public:
	LunchboxSampler( const LunchboxSampler& ) = delete;
	LunchboxSampler& operator=( const LunchboxSampler& other ) = delete;
	static LunchboxSampler& getInstance();
	[[noreturn]] void run();

private:
	LunchboxSampler();
	~LunchboxSampler();
	void setup();
	void setupEventSystem();
	void setupDigitalEvents();
	void setupAnalogEvents();

private:
	std::shared_ptr< EventSystem > m_system;
};
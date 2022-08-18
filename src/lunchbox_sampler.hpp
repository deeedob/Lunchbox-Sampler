#pragma once
#include <Arduino.h>
#include <define_t40.hpp>
#include <event_sytem.hpp>
#include <interrupt_system/digital_interrupts.hpp>
#include <interrupt_system/analog_interrupts.hpp>
#include <base_states.hpp>
#include <audio.hpp>

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
	void setupFSREvents();

private:
	std::shared_ptr<EventSystem> m_system;
	std::unique_ptr<DigitalInterrupts> m_digitalInterrupts;
	std::unique_ptr<AnalogInterrupts> m_analogInterrupts;
	std::unique_ptr<BaseStates> m_states;
};
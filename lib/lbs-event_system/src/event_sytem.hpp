#pragma once
#include <map>
#include <functional>
#include <cstdio>
#include "scheduler.hpp"
#include "events.hpp"
#include "interrupt_system/analog_interrupts.hpp"
#include "interrupt_system/digital_interrupts.hpp"

namespace lbs
{
	struct AnalogData
	{
		/*TODO: better position */
		u_int8_t m_pos;
		u_int16_t m_data;
	};
	
	class EventSystem
	{
		
		using AnalogMapping = std::map< Events::Analog::POTS, std::function< void( AnalogData )>>;
		using DigitalMapping = std::map< Events::DIGITAL, std::function< void()>>;
	public:
		
		EventSystem();
		void attachDigital( Events::DIGITAL e, std::function< void() > f );
		void detachDigital( Events::DIGITAL e );
		void attachAnalog( Events::Analog::POTS e, std::function< void( AnalogData ) > f );
		AnalogMapping& getAnalogMapping();
	
	protected:
		friend class DigitalInterrupts;
		
		friend class AnalogInterrupts;
		
		friend class FSR;
		
		friend class Pots;
		
		void enqueueDigital( Events::DIGITAL e );
		void enqueueAnalog( Events::Analog::POTS e, AnalogData value );
	
	private:
		Scheduler m_scheduler;
		AnalogMapping m_analogMapping;
		DigitalMapping m_digMapping;
	};
}
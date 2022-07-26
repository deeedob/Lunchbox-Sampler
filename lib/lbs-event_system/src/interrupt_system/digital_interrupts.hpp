#pragma once
#include <map>
#include <Arduino.h>
#include <Bounce.h>
#include <Encoder.h>
#include <memory>
#include "events.hpp"
#include "define_t40.hpp"
#include "event_sytem.hpp"

namespace lbs
{
	class EventSystem;
	class DigitalInterrupts
	{
		using DigLookup = std::map< Events::DIGITAL, std::tuple< u_int8_t, void ( * )(), u_int8_t>>;
	public:
		explicit DigitalInterrupts( const std::shared_ptr< EventSystem >& eventSystem, u_int8_t bounceTime = 15 );
		~DigitalInterrupts();
		void enablePin( Events::DIGITAL e );
		void enableCustomizedPin( Events::DIGITAL e, void (* function)(), int mode = CHANGE );
		void enableAll();
		void disablePin( Events::DIGITAL e );
		void disableAll();
		static const DigLookup& getTable();
		/* isr functions */
		static void isrRotaryA();
		static void isrRotaryB();
		static void isrBtnEnter();
		static void isrBtnReturn();
		static void isrBtnToggle();
	private:
		std::shared_ptr< EventSystem > m_eventSystem;
		Bounce m_btnEnter;
		Bounce m_btnReturn;
		Bounce m_btnToggle;
		/* glue routine to have a class like feeling in isr */
		static DigitalInterrupts* m_instance;
	};
}
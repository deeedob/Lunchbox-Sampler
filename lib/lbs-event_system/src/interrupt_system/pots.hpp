#pragma once
#include <array>
#include <cstdio>
#include <memory>

namespace lbs
{
	/*! @class Manages the interrupts for the potentiometers.
	 *  ADC0 is used exclusively to make things simple.
	 *  */

	class AnalogInterrupts;

	class Pots
	{
	public:
		explicit Pots( const AnalogInterrupts* parent, uint8_t pot0, uint8_t pot1, uint8_t pot2, uint8_t pot3,
		               uint16_t delta );
		~Pots();

		static void isr();
		void enableISR( uint8_t prio = 255 );
		void disableISR();
		void update();
		void stopScan();
		uint8_t next();
		uint16_t recalibrateDelta( uint16_t padding, uint16_t samples = 500 );
		uint16_t getDelta() const;
		void setDelta( uint16_t mDelta );

	private:
		void rescanAll();

	private:
		const AnalogInterrupts* const m_parent;
		std::array< volatile uint16_t, 4 > m_values;
		std::array< uint8_t, 4 > m_pots;
		uint8_t m_position;
		uint16_t m_delta;

		static Pots* m_isrInstance;
	};
}// namespace lbs

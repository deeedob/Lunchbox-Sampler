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
		explicit Pots( const std::shared_ptr< AnalogInterrupts >& parent, u_int8_t pot0, u_int8_t pot1, u_int8_t pot2, u_int8_t pot3, u_int16_t delta );
		static void isr();
		void enableISR( u_int8_t prio = 255 );
		void disableISR();
		void update();
		void stopScan();
		u_int8_t next();
		u_int16_t recalibrateDelta( u_int16_t padding, u_int16_t samples = 500 );
		u_int16_t getDelta() const;
		void setDelta( u_int16_t mDelta );
	
	private:
		void rescanAll();
	private:
		std::shared_ptr< AnalogInterrupts > m_parent;
		std::array< volatile u_int16_t, 4 > m_values;
		u_int8_t m_position;
		std::array< u_int8_t, 4 > m_pots;
		u_int16_t m_delta;
		static Pots* m_isrInstance;
	};
}
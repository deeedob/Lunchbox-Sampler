#pragma once
#include "multiplex.hpp"
#include <array>
#include <cstdio>
#include <memory>

namespace lbs
{
	
	/*! @class Manages the interrupts for the FSR Pads.
	 *  ADC1 is used exclusively to make things simpler.
	 * */
	class AnalogInterrupts;
	
	class FSR
	{
		enum PADS
		{
			MPX = 0,
			STATE = 1,
			LOW_BORDER = 2
		};
	public:
		explicit FSR( AnalogInterrupts* parent, u_int8_t mpxPin0, u_int8_t mpxPin1, u_int8_t mpxPin2, u_int8_t mpxPin3, u_int16_t delta );
		~FSR();
		
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
		void updateRange();
	private:
		AnalogInterrupts* const m_parent;   //backlink to parent
		std::array<volatile u_int16_t, 4> m_values;
		volatile u_int8_t m_position;
		/** <Multiplex Control, Triggered On| Off > relates to Note On/Off */
		std::array<std::tuple<Multiplex, bool, int16_t>, 4> m_pads;
		u_int16_t m_delta;
		static FSR* m_isrInstance;
	};
}
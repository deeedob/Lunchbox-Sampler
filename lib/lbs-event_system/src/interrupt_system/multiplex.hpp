#pragma once
#include <functional>
#include <map>
#include <cstdio>
#include <Arduino.h>
#include <define_t40.hpp>

namespace lbs
{
	/*!
   * @class Multiplex is a
   * */
	class Multiplex
	{
	public:
		using MpxLookup = std::map< u_int8_t, std::array< bool, 3>>;
		
		/*! @param mpxPin the multiplexer analog Pin.
		 * */
		explicit Multiplex( u_int8_t mpxPin, u_int8_t sel0, u_int8_t sel1, u_int8_t sel2 );
		Multiplex() = default;
		/*! activate the analog pin on the multiplexer for polling!
		 * */
		void setActive();
		/*! Using the: Construct on first use idiom.
		 *  implementation for the CD74HC4051EE4 analog multiplexer!
		 *  @return the static lookup table of the multiplexer pins
		 * */
		static const MpxLookup& getTable();
	
	private:
		std::function< void() > m_fnPinSelect;
	};
}
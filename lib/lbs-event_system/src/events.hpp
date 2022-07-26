#pragma once

namespace lbs
{
	class Events
	{
	public:
		enum class DIGITAL
		{
			ROTARY_L = 0,
			ROTARY_R,
			ROTARY,
			BTN_ENTER,
			BTN_RETURN,
			BTN_TOGGLE,
			LAST,
		};
		
		class Analog
		{
		public:
			enum class POTS
			{
				POT_0 = 0,
				POT_1 = 1,
				POT_2 = 2,
				POT_3 = 3,
				LAST,
			};
			enum class FSR
			{
				FSR_0 = 0,
				FSR_1 = 1,
				FSR_2 = 2,
				FSR_3 = 3,
				LAST,
			};
		};
	};
}
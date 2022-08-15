#pragma once

namespace lbs
{
	class Events
	{
	public:
		class Analog
		{
		public:
			enum class POTS
			{
				
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
	
	class DIGITAL : public Events
	{
	public:
		static const uint8_t ROTARY_L = 0;
		static const uint8_t ROTARY_R = 1;
		static const uint8_t ROTARY = 2;
		static const uint8_t BTN_ENTER = 3;
		static const uint8_t BTN_RETURN = 4;
		static const uint8_t BTN_TOGGLE = 5;
	};
	
	class ANALOG : public Events
	{
	public:
		class POTS : public Events
		{
			static const uint8_t POT_0 = 0;
			static const uint8_t POT_1 = 1;
			static const uint8_t POT_2 = 2;
			static const uint8_t POT_3 = 3;
		};
	};
}
#pragma once
#include <ADC.h>
#include <memory>
#include "events.hpp"
#include "define_t40.hpp"
#include "pots.hpp"
#include "fsr.hpp"

/* we have big jitter problems when using adc1 with usb
 * https://forum.pjrc.com/threads/45206-Pedvide-s-ADC-library-multiple-channel-simultaneous-amp-continuous-acquisition
 * https://forum.pjrc.com/threads/25532-ADC-library-update-now-with-support-for-Teensy-3-1?p=45376&viewfull=1#post45376
 * */
//NVIC_SET_PRIORITY(IRQ_USBOTG, 200);

namespace lbs
{
	class EventSystem;
	
	class AnalogInterrupts : std::enable_shared_from_this< AnalogInterrupts >
	{
	public:
		explicit AnalogInterrupts( const std::shared_ptr< EventSystem >& eventSystem );
		const std::unique_ptr< Pots >& getPots() const;
		const std::unique_ptr< FSR >& getFSR() const;
		const std::unique_ptr< ADC >& getAdc() const;
		const std::shared_ptr< EventSystem >& getEventSystem() const;
		void enableAll();
		void disableAll();
	
	private:
		std::shared_ptr< EventSystem > m_eventSystem;
		std::unique_ptr< ADC > m_adc;
		std::unique_ptr< Pots > m_pots;
		std::unique_ptr< FSR > m_fsr;
	};
}
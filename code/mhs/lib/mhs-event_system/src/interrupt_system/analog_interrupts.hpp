#pragma once
#include "events.hpp"
#include "define_t40.hpp"
#include <ADC.h>
#include <memory>
#include "pots.hpp"
#include "fsr.hpp"

/* we have big jitter problems when using adc1 with usb
 * https://forum.pjrc.com/threads/45206-Pedvide-s-ADC-library-multiple-channel-simultaneous-amp-continuous-acquisition
 * https://forum.pjrc.com/threads/25532-ADC-library-update-now-with-support-for-Teensy-3-1?p=45376&viewfull=1#post45376
 * */
//NVIC_SET_PRIORITY(IRQ_USBOTG, 200);

namespace lbs {
    volatile ADC _glob_adc;
    class AnalogInterrupts
    {
    public:
        AnalogInterrupts();

        const std::unique_ptr<Pots>& getPots() const;
        const std::unique_ptr<FSR>& getFSR() const;

    private:
        std::unique_ptr<Pots> pots;
        std::unique_ptr<FSR> fsr;
    };
}

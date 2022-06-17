#pragma once
#include "../include/define_t40.hpp"
#include "event_sytem.hpp"
#include <Arduino.h>
#include <ADC.h>
#include <frozen/map.h>
#include <atomic>

namespace mhs
{
    /* for now declared in global... TODO: can this be a member? regarding ISR */
    std::atomic<int> _pinAstateCurrent(LOW);
    std::atomic<int> _pinAstateLast(LOW);
    /* globals to access the adc values */
    std::atomic<std::array<u_int16_t, 4>> _pinAnalogPotOldValue();
    volatile  u_int8_t _analogReadPosition;

    class InterruptHandler
    {
    public:
        /* every pin has at least one of these properties
         * thanks @https://github.com/KurtE/TeensyDocuments/blob/master/Teensy4%20MicroMod%20Pins.pdf
         * Note that the AudioInputAnalog class uses adc0!
         */
        enum class ADC_STATE {
            ADC0 = 0,
            ADC1 = 1,
            BOTH = 2,
        };
        /* All analog interrupts */
        class AnalogInterrupts
        {
        public:
            AnalogInterrupts(u_int16_t _delta = 20);
            ~AnalogInterrupts();

            static void enableAll();
            static void enableADC0();
            static void enableADC1();

            static void disableAll();
            static void disableADC0();
            static void disableADC1();

        private:
            static void readAllOldValues() ;
            void readSinglePot0() const;
            void readSinglePot1() const;
            void readSinglePot2() const;
            void readSinglePot3() const;
            volatile static ADC adc;
            u_int16_t delta;
        };

        /* All digital interrupts of the system */
        class DigitalInterrupts
        {
        public:
            DigitalInterrupts();
            ~DigitalInterrupts();
            static void enablePin( EventSystem::Events::DIGITAL e, void (*function)(void), int mode = CHANGE);
            static void disablePin( EventSystem::Events::DIGITAL e );
        private:
            static void setup_all_isr();
        };

        InterruptHandler();
    private:
        /* todo add volatile */
        static EventSystem m_event_system;
        static std::map<EventSystem::Events::DIGITAL, u_int8_t> m_dig_lookup;
        /* <event/pos <pin, adc>> */
        static constexpr frozen::map<EventSystem::Events::ANALOG, std::pair<u_int8_t, ADC_STATE>, 4> m_analog_lookup = {
                { EventSystem::Events::ANALOG::POT0, {_POT0, InterruptHandler::ADC_STATE::BOTH } },
                { EventSystem::Events::ANALOG::POT1, {_POT1, InterruptHandler::ADC_STATE::BOTH } },
                { EventSystem::Events::ANALOG::POT2, {_POT2, InterruptHandler::ADC_STATE::ADC0 } },
                { EventSystem::Events::ANALOG::POT3, {_POT3, InterruptHandler::ADC_STATE::ADC1 } },
        };
    };
}
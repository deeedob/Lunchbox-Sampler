#pragma once
#include "../include/define_t40.hpp"
#include "event_sytem.hpp"
#include <Arduino.h>
#include <ADC.h>
#include <atomic>
#include <map>
#include "fsr.hpp"

namespace lbs
{
    /* for now declared in global... TODO: can this be a member? regarding ISR */
    std::atomic<int> _pinAstateCurrent(LOW);
    std::atomic<int> _pinAstateLast(LOW);


    class InterruptHandler
    {
    public:

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
            static const anlg_lookup& getAnalogLookup();

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

            using dig_lookup = std::map<EventSystem::Events::DIGITAL, u_int8_t>;
        public:
            DigitalInterrupts();
            ~DigitalInterrupts();
            static void enablePin( EventSystem::Events::DIGITAL e, void (*function)(void), int mode = CHANGE);
            static void disablePin( EventSystem::Events::DIGITAL e );
            static const dig_lookup & getDigitalLookup();
        private:
            static void setup_all_isr();
        };

        InterruptHandler();
    private:
        /* todo add volatile */
        static EventSystem m_event_system;
    };
}
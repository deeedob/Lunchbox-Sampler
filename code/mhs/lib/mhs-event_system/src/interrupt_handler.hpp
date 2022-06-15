#pragma once
#include "abstract_interrupt.hpp"
#include "event_sytem.hpp"
#include <Arduino.h>
#include <ADC.h>
#include "../include/define_t40.hpp"

namespace mhs
{
    /* for now declared in global... TODO: can this be a member? regarding ISR */
    std::atomic<int> _pinAstateCurrent(LOW);
    std::atomic<int> _pinAstateLast(LOW);
    std::atomic<std::array<u_int16_t, 4>> _pinAnalogOldPot();
    class InterruptHandler
    {
    public:
        /* All analog interrupts */
        class AnalogInterrupts
        {
        public:
            AnalogInterrupts();
            ~AnalogInterrupts();

            void enablePin( EventSystem::Events::ANALOG e, void (*function)(void), int mode = CHANGE );
            void disablePin( EventSystem::Events::ANALOG e );

        private:
            void setup_all_isr();
            void readAllOldValues();
        };

        /* All digital interrupts of the system */
        class DigitalInterrupts
        {
        public:
            DigitalInterrupts();
            DigitalInterrupts(std::initializer_list<std::pair<EventSystem::Events::DIGITAL, u_int8_t>> list);
            ~DigitalInterrupts();
            void enablePin( EventSystem::Events::DIGITAL e, void (*function)(void), int mode = CHANGE);
            void disablePin( EventSystem::Events::DIGITAL e );
        private:
            static void setup_all_isr();
        };

    private:
        static EventSystem m_event_system;
        static std::map<EventSystem::Events::DIGITAL, u_int8_t> m_dig_lookup;
        static std::map<EventSystem::Events::ANALOG, u_int8_t> m_analog_lookup;
    };
}
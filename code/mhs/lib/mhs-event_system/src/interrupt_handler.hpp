#pragma once
#include "abstract_interrupt.hpp"
#include "event_sytem.hpp"
#include <Arduino.h>
#include "../include/define_t40.hpp"

namespace mhs
{
    /* for now declared in global... TODO: can this be a member? regarding ISR */
    int __pinAstateCurrent = LOW;
    int __pinAstateLast = LOW;
    class InterruptHandler
    {
    public:
        /* All analog interrupts */
        class AnalogInterrupts : AbstractInterrupt
        {
        public:
            explicit AnalogInterrupts(std::initializer_list<EventSystem::EventInfo> mapping );
            ~AnalogInterrupts() override;

            void enablePin( EventSystem::Events e ) override;
            void disablePin( EventSystem::Events e ) override;

        private:
            void setup_isr(EventSystem::Events::ANALOG event_anlg);
        };

        /* All digital interrupts of the system */
        class DigitalInterrupts : AbstractInterrupt
        {
        public:
            DigitalInterrupts(std::initializer_list<EventSystem::EventInfo> mapping );
            ~DigitalInterrupts() override;
            void enablePin( EventSystem::Events e ) override;
            void disablePin( EventSystem::Events e ) override;
        private:
            void setup_isr( EventSystem::EventInfo event_dig);
        };

    private:
        static EventSystem m_eventSystem;
    };
}
#pragma once
#include "../include/define_t40.hpp"
#include "abstract_interrupt.hpp"
#include <Arduino.h>

namespace mhs
{
    class InterruptHandler
    {
    public:
        class AnalogInterrupts : AbstractInterrupt
        {
        public:
            AnalogInterrupts();
            ~AnalogInterrupts() override;

            void enableAll() override;
            void disableAll() override;
            void enablePin( unsigned short pin ) override;
            void disablePin( unsigned short pin ) override;

        private:
            void setup_isr() override;
        };

        class DigitalInterrupts : AbstractInterrupt
        {
        public:
            DigitalInterrupts();
            ~DigitalInterrupts() override;

            void enableAll() override;
            void disableAll() override;
            void enablePin( unsigned short pin ) override;
            void disablePin( unsigned short pin ) override;
        private:
            void setup_isr() override;
        };

        InterruptHandler();

        InterruptHandler( const InterruptHandler & ) = delete;

        void operator=( InterruptHandler & ) = delete;

        void enableGlobal();

        void disableGlobal();

    private:
        void init();
    };

}
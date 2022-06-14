#pragma once
#include <map>
#include <functional>
#include <cstdio>
#include "interrupt_handler.hpp"
#include "scheduler.hpp"
namespace mhs
{
    class EventSystem
    {
    public:
        class Events
        {
        public:
            struct DIGITAL
            {
                enum class ROTARY
                {
                    LEFT,
                    RIGHT
                };
                enum class BUTTON
                {
                    ENTER,
                    RETURN,
                    TOGGLE
                };

                enum class MIDI
                {
                    SERIAL_,
                    FSR,
                    HOST
                };

            };
            struct ANALOG
            {
                enum class POTI
                {
                    ONE,
                    TWO,
                    THREE,
                    FOUR
                };

                enum class FSR
                {
                    ONE,
                    TWO,
                    THREE,
                    FOUR
                };
            };

        };

        struct EventInfo
        {
            u_int16_t pin;
            Events event;
        };

        void attach( u_int16_t p, const Events &e, std::function<void()> f );

        void attach( EventInfo eventInfo, std::function<void()> f );

        void detach( EventInfo eventInfo );

        void detach( u_int16_t p );

    protected:
        void enqueueDigital( Events e);
        void enqueueAnalog( Events e);

    private:
        friend class InterruptHandler;
        Scheduler scheduler;
        std::map<EventInfo, std::function<void()>> eventTable;
    };
}
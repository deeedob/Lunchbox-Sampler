#pragma once
#include <map>
#include <functional>
#include <cstdio>
#include "scheduler.hpp"
namespace lbs
{
    class EventSystem
    {
    public:

        struct EventInfo
        {
            u_int16_t pin;
            Events event;
        };

        EventSystem() = default;

        void attach( u_int16_t p, const Events &e, std::function<void()> f );

        void attach( EventInfo eventInfo, std::function<void()> f );

        void detach( EventInfo eventInfo );

        void detach( u_int16_t p );

    protected:
        friend class InterruptHandler;
        void enqueueDigital( Events::DIGITAL e);
        void enqueueAnalog( Events::POTS e);

    private:
        Scheduler scheduler;
        std::map<Events::DIGITAL, std::function<void()>> m_digMapping;
        std::map<Events::POTS, std::function<void()>> m_analogMapping;
    };
}
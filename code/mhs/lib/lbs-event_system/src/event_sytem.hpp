#pragma once
#include <map>
#include <functional>
#include <cstdio>
#include "scheduler.hpp"
#include "events.hpp"
namespace lbs
{
    class EventSystem
    {
    public:

        EventSystem();

        void attachDigital(Events::DIGITAL e, std::function<void()> f);
        void detachDigital(Events::DIGITAL e);
        void attachAnalog(Events::Analog::POTS e, std::function<void(u_int16_t)> f);

    protected:
        friend class DigitalInterrupts;
        friend class AnalogInterrupts;
        void enqueueDigital( Events::DIGITAL e);
        void enqueueAnalog( Events::Analog::POTS e, u_int16_t value);

    private:
        Scheduler scheduler;
        std::map<Events::DIGITAL, std::function<void()>> m_digMapping;
        std::map<Events::Analog::POTS, std::function<void(u_int16_t val)>> m_analogMapping;
    };
}
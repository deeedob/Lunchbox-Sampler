#pragma once
#include "active_object/worker.hpp"
#include <TeensyThreads.h>

namespace lbs
{
    class Scheduler
    {
    public:
        Scheduler();

        Scheduler( Scheduler & ) = delete;
        void operator=( Scheduler & ) = delete;

        void dispatch();
        void enqueue();

    private:
        friend class EventSystem;
        Worker<std::function<void()>> m_digitalListener;
        Worker<std::function<void(u_int16_t value)>> m_analogListener;
        Worker<std::function<void()>> m_midiListener;
    };
}
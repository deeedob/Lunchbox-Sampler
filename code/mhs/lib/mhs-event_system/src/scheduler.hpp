#pragma once
#include "active-object/worker.hpp"
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
        Worker m_digitalListener;
        Worker m_analogListener;
        Worker m_audio;
    };
}
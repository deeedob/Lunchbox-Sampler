#pragma once
#include "active-object/worker.hpp"
#include <TeensyThreads.h>
#include "event_sytem.hpp"

namespace mhs
{
    class Scheduler
    {
    public:
        Scheduler();

        Scheduler( Scheduler & ) = delete;
        void operator=( Scheduler & ) = delete;

        void dispatch();
        void enqueue();

    protected:
        friend class EventSystem;
    private:
        Worker m_digitalListener;
        Worker m_analogListener;
        Worker m_audio;
    };
}
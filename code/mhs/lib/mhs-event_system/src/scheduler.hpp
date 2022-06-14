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

    private:
        Worker<std::function<void>> m_activeObject;
    };
}
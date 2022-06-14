#pragma once
#include <atomic>
#include <memory>
#include <TeensyThreads.h>
#include "dispatch_queue.hpp"
#include "runnable.hpp"

namespace mhs
{
    template<class Func, class ...Args>
    class Worker : public Runnable
    {
    public:
        Worker();
        ~Worker();

    protected:
        void runTarget( void *arg ) override;

    public:
        Worker( const Worker & ) = delete;
        void operator=( Worker & ) = delete;

        void send( const Func& f );

    private:
        std::atomic<bool> done;
        std::unique_ptr<std::thread> runnable;
        DispatchQueue<Func> dispatchQueue;
    };
}
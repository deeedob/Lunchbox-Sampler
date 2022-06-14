#pragma once
#include <atomic>
#include <memory>
#include <TeensyThreads.h>
#include "dispatch_queue.hpp"
#include "runnable.hpp"
#include "../scheduler.hpp"
namespace mhs
{
    class Worker : public Runnable
    {
    public:

        Worker();
        Worker( const Worker & ) = delete;
        void operator=( Worker & ) = delete;
        ~Worker();

        void send( const std::function<void()>& f );

    protected:
        void runTarget( void *arg ) override;

    private:
        friend class Scheduler;
        std::atomic<bool> done;
        std::unique_ptr<std::thread> runnable;
        DispatchQueue<std::function<void()>> dispatchQueue;
    };
}
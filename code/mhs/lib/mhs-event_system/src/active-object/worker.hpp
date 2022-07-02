#pragma once
#include <atomic>
#include <memory>
#include <TeensyThreads.h>

#include "dispatch_queue.hpp"
#include "runnable.hpp"

namespace lbs
{
    class Worker : public Runnable
    {
    public:

        Worker();
        Worker( const Worker & ) = delete;
        void operator=( Worker & ) = delete;
        ~Worker();

        void runTarget( void *arg ) override;
        void send( const std::function<void()>& f );
        inline int getThreadId() { return runnable->get_id(); }


    private:
        std::atomic<bool> done;
        std::unique_ptr<std::thread> runnable;
        DispatchQueue<std::function<void()>> dispatchQueue;
    };
}
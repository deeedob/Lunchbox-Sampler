#pragma once
#include <atomic>
#include <memory>
#include <TeensyThreads.h>

#include "dispatch_queue.hpp"
#include "runnable.hpp"

namespace lbs
{
    template<class Func, class Args = void*>
    class Worker : public Runnable
    {

        static_assert(
                (std::is_base_of<std::function<void()>, Func>::value) ||
                (std::is_base_of<std::function<void(u_int16_t)>, Func>::value),
                "Func must be a function");

    public:
        Worker(int wait_time = 50) : done(false), dispatchQueue(wait_time) {
            runnable = std::make_unique<std::thread>(&Runnable::runThread, this);
            //runnable->detach();
        }
        ~Worker() override {
            done = true;
            runnable->join();
        };
        Worker( const Worker & ) = delete;
        void operator=( Worker & ) = delete;

        void runTarget( void *arg ) override {
            while( !done ) {
                auto element = dispatchQueue.take();
                element.f(arg);
            }
        };
        void send( const Func& f, const Args& args) {
            dispatchQueue.put({ f, args });
        };
        int getThreadId() {
            return runnable->get_id();
        };

    private:
        std::atomic<bool> done;
        std::unique_ptr<std::thread> runnable;
        DispatchQueue<Func, Args> dispatchQueue;
    };
}
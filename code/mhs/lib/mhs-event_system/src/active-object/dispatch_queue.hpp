#pragma once

#include <queue>
#include <functional>
#include <atomic>
#include <TeensyThreads.h>

namespace mhs
{
    /*!
     * @ingroup EventSystem
     * This Class is a thread-safe wrapper around std::queue.
     * A Thread yields until an event arrives and then processes it.
     */
    template<class Func, class ...Args>
    class DispatchQueue
    {
    public:
        explicit DispatchQueue(u_int32_t _wait_us) : wait_us(_wait_us) { };

        void put( const Func &f ) volatile;
        Func take();

        u_int32_t getWaitUs() const;
        void setWaitUs( u_int32_t waitUs );

    private:
        std::mutex mtx;
        std::queue<Func> queue;
        u_int32_t wait_us;
    };
}
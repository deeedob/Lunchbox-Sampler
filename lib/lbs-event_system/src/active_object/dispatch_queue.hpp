#pragma once

#include <queue>
#include <functional>
#include <atomic>
#include <TeensyThreads.h>
#include <cstdio>
#include <Arduino.h>
#include <midi_Defs.h>
#include <usb_midi.h>

namespace lbs
{
    /*!
     * @ingroup EventSystem
     * This Class is a thread-safe wrapper around std::queue.
     * A Thread yields until an event arrives and then processes it.
     */
    template<class Func, class Args = void*>
    class DispatchQueue
    {
        static_assert((std::is_base_of<std::function<void()>, Func>::value) ||
        (std::is_base_of<std::function<void(u_int16_t)>, Func>::value),
        "Func must be a function");

        struct Pack {
            Func f;
            Args args;
        };

    public:

        explicit DispatchQueue(u_int32_t wait_us) : m_wait_us( wait_us ){};

        void put( const Pack& pack ) {
            std::lock_guard<std::mutex> guard(m_mtx);
            m_queue.push(pack);
        };

        Pack take() {
            while( m_queue.empty() ) {
                threads.delay_us(m_wait_us);         // let the thread wait with yield() until the queue has content
            }
            std::lock_guard<std::mutex> guard(m_mtx);
            auto p = m_queue.front();
            m_queue.pop();
            return p;
        };

        u_int32_t getWaitUs() const {
            return m_wait_us;
        };

        void setWaitUs( u_int32_t waitUs ) {
            m_wait_us = waitUs;
        };

    private:
        std::mutex m_mtx;
        std::queue<Pack> m_queue;
        u_int32_t m_wait_us;
    };
}
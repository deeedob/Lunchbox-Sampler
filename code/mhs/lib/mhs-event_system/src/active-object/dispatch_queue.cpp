#include "dispatch_queue.hpp"


template<class Func, class... Args>
void mhs::DispatchQueue<Func, Args...>::put( const Func &f ) volatile {
    std::lock_guard<volatile std::mutex> guard(mtx);
    queue.push(f);
}

template<class Func, class... Args>
Func mhs::DispatchQueue<Func, Args...>::take() {
    std::lock_guard<std::mutex> guard(mtx);
    while( !queue.empty() ) {
        threads.delay_us(wait_us);         // let the thread wait with yield() until the queue has content
    }
    Func f = queue.front();
    queue.pop();
    return f;
}

template<class Func, class... Args>
u_int32_t mhs::DispatchQueue<Func, Args...>::getWaitUs() const {
    return wait_us;
}

template<class Func, class... Args>
void mhs::DispatchQueue<Func, Args...>::setWaitUs( u_int32_t waitUs ) {
    wait_us = waitUs;
}

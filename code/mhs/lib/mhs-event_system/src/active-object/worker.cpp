#include "worker.hpp"

template<class Func, class... Args>
mhs::Worker<Func, Args...>::Worker() : done(false), dispatchQueue(50) {
    runnable = std::make_unique<std::thread>(&Runnable::runThread, this);
    runnable->detach();
}

template<class Func, class... Args>
mhs::Worker<Func, Args...>::~Worker() {
    done = true;
    runnable->join();
}

template<class Func, class... Args>
void mhs::Worker<Func, Args...>::send( const Func &f ) {
    dispatchQueue.put(f);
}

template<class Func, class... Args>
void mhs::Worker<Func, Args...>::runTarget( void *arg ) {
    while( !done ) {
        Func f = dispatchQueue.take();
        f();
    }
}

#include "worker.hpp"

mhs::Worker::Worker() : done(false), dispatchQueue(50) {
    runnable = std::make_unique<std::thread>(&Runnable::runThread, this);
    runnable->detach();
}

mhs::Worker::~Worker() {
    done = true;
    runnable->join();
}

void mhs::Worker::send( const std::function<void()> &f ){
    dispatchQueue.put(f);
}

void mhs::Worker::runTarget( void *arg ) {
    while( !done ) {
        std::function<void()> f = dispatchQueue.take();
        f();
    }
}

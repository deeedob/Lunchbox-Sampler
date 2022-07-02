#include "worker.hpp"

lbs::Worker::Worker() : done(false), dispatchQueue(50) {
    runnable = std::make_unique<std::thread>(&Runnable::runThread, this);
    runnable->detach();
}

lbs::Worker::~Worker() {
    done = true;
    runnable->join();
}

void lbs::Worker::send( const std::function<void()> &f ){
    dispatchQueue.put(f);
}

void lbs::Worker::runTarget( void *arg ) {
    while( !done ) {
        std::function<void()> f = dispatchQueue.take();
        f();
    }
}

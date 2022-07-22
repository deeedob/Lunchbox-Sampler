#pragma once

/**
 * Runnable is an abstract Interface that gives us the opportunity
 * to input the static method of its class to a thread.
 * */
class Runnable {
protected:
    virtual void runTarget(void* arg) = 0;
public:
    virtual ~Runnable() = default;
    static void runThread(void* arg) {
        auto* runnable = static_cast<Runnable*>(arg);
        runnable->runTarget(arg);
    }
};
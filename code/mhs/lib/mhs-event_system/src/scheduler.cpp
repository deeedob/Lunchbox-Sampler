#include "scheduler.hpp"

mhs::Scheduler::Scheduler() {
    threads.setSliceMicros(10); // set global slicing
    threads.setTimeSlice(m_audio.getThreadId(), 5); // audio threads get 5x time
}

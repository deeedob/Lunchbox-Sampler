#include "scheduler.hpp"

lbs::Scheduler::Scheduler()
	: m_digitalWorker( 5000 ), m_analogWorker( 5000 )
{
	threads.setSliceMicros(10); // set global slicing
	threads.setTimeSlice( m_digitalWorker.getThreadId(), 5); // digital threads gets 5x time
}
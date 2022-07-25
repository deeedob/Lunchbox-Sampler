#include "scheduler.hpp"

lbs::Scheduler::Scheduler()
	: m_digitalListener( 5000 )//, m_analogListener( 5000 )
{
	/* todo: how to handle ui and audio? */
	//threads.setSliceMicros(10); // set global slicing
	//threads.setTimeSlice(m_audioWorker.getThreadId(), 5); // audio threads get 5x time
}
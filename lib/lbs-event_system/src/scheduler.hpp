#pragma once
#include "active_object/worker.hpp"
#include <TeensyThreads.h>
#include <midi_listener.hpp>

namespace lbs
{
	class Scheduler
	{
	public:
		Scheduler();
		Scheduler( Scheduler& ) = delete;
		void operator=( Scheduler& ) = delete;
		void dispatch();
	
	private:
		friend class EventSystem;
		
		Worker< std::function< void() >> m_digitalWorker;
		Worker< std::function< void() >> m_analogWorker;
	};
}
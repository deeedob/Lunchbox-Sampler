#pragma once
#include <atomic>
#include <memory>
#include <TeensyThreads.h>

#include "dispatch_queue.hpp"
#include "runnable.hpp"

namespace lbs
{
	template<class Func>
	class Worker : public Runnable
	{
	
	public:
		explicit Worker( int waitTime = 50 )
			: m_done( false ), m_dispatchQueue( waitTime )
		{
			m_runnable = std::make_unique<std::thread>( &Runnable::runThread, this );
			m_runnable->detach();
		}
		
		~Worker() override
		{
			m_done = true;
			m_runnable->join();
		};
		Worker( const Worker& ) = delete;
		void operator=( Worker& ) = delete;
		
		void runTarget( void* arg ) override
		{
			while ( !m_done ) {
				Func item = m_dispatchQueue.take();
				item();
			}
		};
		
		void send( Func f )
		{
			m_dispatchQueue.put( f );
		};
		
		int getThreadId()
		{
			return m_runnable->get_id();
		};
	
	private:
		std::atomic<bool> m_done;
		std::unique_ptr<std::thread> m_runnable;
		DispatchQueue<Func> m_dispatchQueue;
	};
}
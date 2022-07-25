#pragma once

#include <queue>
#include <functional>
#include <atomic>
#include <TeensyThreads.h>
#include <cstdio>
#include <Arduino.h>
#include <midi_Defs.h>
#include <usb_midi.h>

namespace lbs
{
	/*!
	 * @ingroup EventSystem
	 * This Class is a thread-safe wrapper around std::queue.
	 * A Thread yields until an event arrives and then processes it.
	 */
	template< class Func, class Args = void* >
	class DispatchQueue
	{
		//static_assert(( std::is_base_of< std::function< void() >, Func >::value ) || ( std::is_base_of< std::function< void( u_int16_t ) >, Func >::value ), "Func must be a function" );
	
	public:
		explicit DispatchQueue( u_int32_t waitUs )
			: m_waitUs( waitUs ) { };
		
		void put( const Func& pack )
		{
			std::lock_guard< std::mutex > guard( m_mtx );
			m_queue.push( pack );
		};
		
		Func take()
		{
			while( m_queue.empty()) {
				threads.delay_us( m_waitUs );         // let the thread wait with yield() until the queue has content.
			}
			std::lock_guard< std::mutex > guard( m_mtx );
			auto p = m_queue.front();
			m_queue.pop();
			return p;
		};
		
		u_int32_t getWaitUs() const
		{
			return m_waitUs;
		};
		
		void setWaitUs( u_int32_t waitUs )
		{
			m_waitUs = waitUs;
		};
	
	private:
		std::mutex m_mtx;
		std::queue< Func > m_queue;
		u_int32_t m_waitUs;
	};
}
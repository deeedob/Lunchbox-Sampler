#pragma once

#include <vector>
#include <TeensyThreads.h>
#include <algorithm>

/*!
 * A header for serving a thread-safe observer implementation!
 * */

/*!
 * @brief Inherit from this class as a Observer. Get notified through the emit function.
 * @tparam T The class to observe.
 * */
template<class T>
struct Observer
{
	/*!
	 * @brief gets called from an observable when notifying!
	 * @param src The Observable object.
	 */
	virtual void emit( const T& src ) = 0;
};

/*!
 * @brief Make a class Observable from a main observer.
 * @tparam T The class to observe
 */
template<class T>
struct Observable
{
public:
	/*!
	 * @brief When implemented trigger this function to notify the Observer.
	 * @param src Usually the *this pointer or the class that gets Observed.
	 */
	void notify( const T& src )
	{
		m_mutex.lock();
		for( const auto& i : m_observers ) {
			i->emit( src );
		}
		m_mutex.unlock();
	}
	
	/*!
	 * @brief Subscribe to the observer.
	 * @param o The observer to watch on this class.
	 */
	void subscribe( Observer<T>* o )
	{
		m_mutex.lock();
		m_observers.push_back( o );
		m_mutex.unlock();
	}
	
	/*!
	 * @brief Unsubscribe from the observer.
	 * @param o The Observer who is watching this class.
	 */
	void unsubscribe( Observer<T>* o )
	{
		m_mutex.lock();
		m_observers.erase( std::remove( m_observers.begin(), m_observers.end(), o ), m_observers.end());
		m_mutex.unlock();
	}

private:
	std::vector<Observer<T>*> m_observers;
	std::mutex m_mutex;
};
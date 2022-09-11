#include <events.hpp>
#include <map>
#include <functional>

namespace lbs
{
	template<class State, class Event, class FuncType, class Data = int>
	class TransitionTable
	{
	public:
		using StringType = String;
		using Transitions = std::vector<std::pair<Event, State>>;
	public:
		TransitionTable() = default;
		
		void addTransition( const State& state, Transitions&& t )
		{
			m_transitionTable.at( state ).emplace_back( std::move( t ));
		}
		
		void setFunction( const State& state, std::function<FuncType>&& f )
		{
			m_functions[ state ] = std::move( f );
		}
		
		void triggerFunction( const State& state, Graphics* g )
		{
			m_functions.at( state )( g );
		}
		
		void triggerCurrentFunction()
		{
			m_functions.at( m_current )();
		}
		
		State transition( Event e, bool use_selection )
		{
			Transitions trans = m_transitionTable.at( m_current );
			// use the internal selection from the states
			if( use_selection ) {
				if( trans[ m_selection ].first == e ) {
					m_current = trans[ m_selection ].second;
					m_selection = 0;
				}
			} else { // or find the first occurrence
				for( int i = 0; i < trans.size(); i++ ) {
					if( trans[ i ].first == e ) {
						m_current = trans[ i ].second;
						m_selection = 0;
						break;
					}
				}
			}
			
			return m_current;
		}
		
		int getCurrentVecSize() const
		{
			return m_transitionTable.at( m_current ).size();
		}
		
		void setSelectionSize( unsigned int size )
		{
			m_selectionSize = size;
		}
		
		int selected() const
		{
			return m_selection;
		}
		
		State selectedState() const
		{
			Transitions trans = m_transitionTable.at( m_current );
			return trans[ m_selection ].second;
		}
		
		int moveLeft()
		{
			m_selection--;
			if( m_selection < 0 )
				m_selection = m_selectionSize - 1;
			return m_selection;
		}
		
		int moveRight()
		{
			m_selection++;
			if( m_selection >= m_selectionSize )
				m_selection = 0;
			return m_selection;
		}
		
		StringType name( const State& state )
		{
			auto v = m_names.find( state );
			if( v == m_names.end())
				return "";
			return v->second;
		}
		
		StringType mainName()
		{
			return name( m_current );
		}
		
		StringType subName()
		{
			auto t = m_transitionTable.at( m_current );
			return name( t[ m_selection ].second );
		}
		
		void setName( const State& state, const StringType& name )
		{
			m_names[ state ] = name;
		}
		
		std::vector<StringType> allNamesCurrent()
		{
			std::vector<StringType> temp;
			auto it = m_transitionTable.find( m_current );
			if( it != m_transitionTable.end()) {
				for( const auto& i : it->second ) {
					temp.push_back( name( i.second ));
				}
			}
			
			return temp;
		}
		
		std::map<State, Transitions>& get()
		{
			return m_transitionTable;
		}
		
		const State& current()
		{
			return m_current;
		}
		
		void setCurrent( const State& state )
		{
			m_current = state;
		}
		
		void addData( const State& state, std::vector<Data>&& d )
		{
			for( int i = 0; i < m_data.size(); i++ ) {
				if( m_data[ i ].first == state ) {
					m_data[ i ].second.insert( m_data[ i ].second.end(), d.begin(), d.end());
					return;
				}
			}
			m_data.push_back( { state, d } );
		}
		
		const std::vector<Data>& getData( const State& state )
		{
			for( const auto& i : m_data ) {
				if( i.first == state )
					return i.second;
			}
		}
	
	private:
		std::map<State, Transitions> m_transitionTable;
		std::map<State, StringType> m_names;
		std::map<State, std::function<FuncType>> m_functions;
		std::vector<std::pair<State, std::vector<Data>>> m_data;
		State m_current;
		int m_selection { 0 };
		int m_selectionSize { 0 };
		
	};
}
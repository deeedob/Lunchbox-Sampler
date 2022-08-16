#include "base_states.hpp"
#include <define_t40.hpp>

BaseStates::BaseStates()
{
	m_graphics = std::make_unique<Graphics>( C_DISPLAY_WIDTH, C_DISPLAY_HEIGHT, &Wire1, C_DISPLAY_RST, 1000000, 1000000 );
	m_currentState = State::MAIN_MENU;
	//m_transitionList.push_back(
	//	{{ "MAIN MENU", State::MAIN_MENU }, ModuleLoad { }} );
	//m_transitionList.push_back(
	//	{{ "LOAD", State::LOAD }, ModuleLoad { }} );
	//m_transitionList.push_back(
	//	{{ "MANAGE SOUND", State::MANAGESOUND }, ModuleLoad { }} );
	//m_transitionList.push_back(
	//	{{ "AUDIO", State::AUDIO }, ModuleLoad { }} );
	//m_transitionList.push_back(
	//	{{ "MAIN SETTINGS", State::MAINSETTINGS }, ModuleLoad { }} );
}
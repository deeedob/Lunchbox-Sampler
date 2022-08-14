#include "ui.hpp"
#include <define_t40.hpp>

using namespace lbs;

UI::UI()
{
	m_graphics = std::make_unique<Graphics>( C_DISPLAY_WIDTH, C_DISPLAY_HEIGHT, &Wire1, C_DISPLAY_RST, 1000000, 1000000 );
	m_currentState = State::LOAD;
	m_transitionTable[ State::LOAD ] = {
		{ Trigger::setManagesound, State::MANAGESOUND }
	};
	m_transitionTable[ State::MANAGESOUND ] = {
		{ Trigger::setLoad, State::LOAD },
		{ Trigger::setAudio, State::AUDIO }
	};
	m_transitionTable[ State::AUDIO ] = {
		{ Trigger::setManagesound, State::MANAGESOUND },
		{ Trigger::setRecord, State::RECORD }
	};
	m_transitionTable[ State::RECORD ] = {
		{ Trigger::setAudio, State::AUDIO },
		{ Trigger::setMainsettings, State::MAINSETTINGS }
	};
	m_transitionTable[ State::MAINSETTINGS ] = {
		{ Trigger::setRecord, State::RECORD },
	};
}
#pragma once
#include <window.hpp>
#include "../include/define_t40.hpp"
#include <map>

class UI
{
public:
	enum class State
	{
		LOAD,
		AUDIO,
		MANAGESOUND,
		RECORD,
		MAINSETTINGS
	};
	enum class Trigger
	{
		setLoad,
		setAudio,
		setManagesound,
		setRecord,
		setMainsettings
	};
	
	explicit UI( u_int8_t w = 128, u_int8_t h = 128, TwoWire* wire = &Wire1, int8_t display_rst = C_DISPLAY_RST, uint32_t preclk = 400000, uint32_t postclk = 100000 );
	void start();

private:
	u_int8_t statePos = 0;
	State currentState { State::LOAD };
	std::map< State, std::vector< std::pair< Trigger, State>> > transitionTable;
	std::unique_ptr< Window > display;
};
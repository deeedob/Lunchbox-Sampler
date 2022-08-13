#pragma once
#include <map>
#include <vector>
#include <memory>
#include <Arduino.h>
#include "graphics.hpp"

namespace lbs
{
	
	class UI
	{
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
		using Transitions = std::map<State, std::vector<std::pair<Trigger, State>>>;
	public:
		UI();
		void start();
	private:
		Transitions m_transitionTable;
		std::unique_ptr<Graphics> m_graphics;
	};
}
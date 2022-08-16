#pragma once
#include "base_states.hpp"

namespace lbs
{
	class AbstractModule
	{
	public:
		/* enable*/
		virtual void enter( Graphics* g ) = 0;
		virtual void update( Graphics* g ) = 0;
		virtual void exit() = 0;
		virtual ~AbstractModule() = 0;
	};
}
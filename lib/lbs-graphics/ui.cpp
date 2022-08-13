#include "ui.hpp"
#include <define_t40.hpp>

using namespace lbs;

UI::UI()
{
	m_graphics = std::make_unique<Graphics>( 128, 128, &Wire1, C_DISPLAY_RST, 1000000, 1000000 );
}
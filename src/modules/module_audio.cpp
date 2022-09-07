#include "module_audio.hpp"


using namespace lbs;
ModuleAudio::ModuleAudio()
	: AbstractModule( "Audio" )
{
	auto split_screens = WindowSize::createSplitScreen( UTIL::HORIZONTAL, 0.15f );
	m_top = Window( split_screens.first );
	m_bottom=Window( split_screens.second );
	auto split_screens2 = WindowSize::createSplitScreen( UTIL::VERTICAL, 0.5 );
	m_left = Window( split_screens2.first );
	m_right=Window( split_screens2.second );
	m_mainStates.push_back(STATE::AUDIO_MODULE_MAIN);
	m_mainStates.push_back(STATE::AUDIO_MODULE_SUB_1);
	m_currentMainState=m_mainStates.begin();

	m_channels.push_back(STATE::CHANNEL_MASTER);
	m_channels.push_back(STATE::CHANNEL_1);
	m_channels.push_back(STATE::CHANNEL_2);
	m_channels.push_back(STATE::CHANNEL_3);
	m_channels.push_back(STATE::CHANNEL_4);
	m_currentChannel = m_channels.begin();
	
	m_effects.push_back(STATE::EFFECT_VOLUME);
	m_effects.push_back(STATE::EFFECT_FILTER);
	m_effects.push_back(STATE::EFFECT_DELAY);
	m_effects.push_back(STATE::EFFECT_COMPRESSOR);
	m_currentEffect=m_effects.begin();
}

ModuleAudio::~ModuleAudio()
{

}

void ModuleAudio::enter( Graphics* g )
{
	draw_audio_module_main(g);
}
void ModuleAudio::draw_audio_module_sub1( Graphics* g )
{
	g->clearDisplay();
	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_top.printlnCentered( "MASTER" );
	m_left.fillScreen(0x00);
	m_right.fillScreen(0x00);
	m_left.drawLine(0,74,64,74,0xFF);
	m_right.drawLine(0,74,64,74,0xFF);
	
	
	//ROT0
	uint16_t color1_rot0 =0xFF;
	uint16_t color2_rot0 =0x00;
	if(m_currentEffect==m_effects.begin())
	{
		m_left.fillRect(0,0,74,74,0xEE);
		color1_rot0 =0x33;
		color2_rot0 =0xFF;
	}
	m_last_value_rot0 = analogRead( 14 );
	float p = m_last_value_rot0 / 1023 * 100;
	Serial.println(p);
	
	m_left.setTextColor( color1_rot0 );
	m_left.setCursor(14,22);
	m_left.setFont();
	m_left.println("Volume");
	
	m_left.drawRotary( 32, 46, 15, 0xAA, color1_rot0 );
	m_left.drawCircle(32,46,15, color1_rot0, color2_rot0, p);
	
	m_left.setTextColor( color1_rot0 );
	m_left.setCursor(22,64);
	m_left.setFont();
	m_left.println("MIX");

	//ROT1
	uint16_t color1_rot1=0xFF;
	uint16_t color2_rot1=0x00;
	if(m_currentEffect==m_effects.begin()+1)
	{
		m_right.fillRect(0,0,74,74,0xEE);
		color1_rot1=0x33;
		color2_rot1=0xFF;
	}
	m_right.setTextColor(color1_rot1);
	m_right.setCursor(14,22);
	m_right.setFont();
	m_right.println("Filter");
	
	m_last_value_rot1 = analogRead( 15 );
	float p_rot1 = m_last_value_rot1 / 1023 * 100;
	
	m_right.drawRotary( 32, 46, 15, 0xAA , color1_rot1);
	m_right.drawCircle(32,46,15,color1_rot1, color2_rot1, p_rot1);
	m_right.setTextColor(color1_rot1);
	m_right.setCursor(22,64);
	m_right.setFont();
	m_right.println("MIX");
	
	//ROT2
	uint16_t color1_rot2 =0xFF;
	uint16_t color2_rot2 =0x00;
	if(m_currentEffect==m_effects.begin()+2)
	{
		m_left.fillRect(0,74,74,74,0xEE);
		color1_rot2 =0x33;
		color2_rot2 =0xFF;
	}
	m_left.setTextColor(color1_rot2);
	m_left.setCursor(16,77);
	m_left.setFont();
	m_left.println("Delay");
	
	m_left.drawRotary( 32, 103, 15, 0xAA, color1_rot2 );
	m_left.drawCircle(32,103,15, color1_rot2, color2_rot2, 20);
	
	m_left.setTextColor(color1_rot2);
	m_left.setCursor(22,119);
	m_left.setFont();
	m_left.println("MIX");
	
	
	//ROT3
	uint16_t color1_rot3=0xFF;
	uint16_t color2_rot3=0x00;
	if(m_currentEffect==m_effects.begin()+3)
	{
		m_right.fillRect(0,74,74,74,0xEE);
		color1_rot3=0x33;
		color2_rot3=0xFF;
	}
	m_right.setTextColor(color1_rot3);
	m_right.setCursor(21,77);
	m_right.setFont();
	m_right.println("Comp");
	
	m_right.drawRotary( 32, 103, 15, 0xAA, color1_rot3 );
	m_right.drawCircle(32,103,15, color1_rot3, color2_rot3, 20);
	m_right.setTextColor(color1_rot3);
	m_right.setCursor(22,119);
	m_right.setFont();
	m_right.println("MIX");
	
	g->drawWindow( m_left );
	g->drawWindow( m_right );
	g->drawWindow( m_top );

	g->display();
	
}
void ModuleAudio::update( Graphics* g, Events::DIGITAL e )
{
	if(m_currentMainState==m_mainStates.begin())
	{
		if( e == Events::DIGITAL::ROTARY_R ) {
			m_currentChannel++;
			if( m_currentChannel == m_channels.end() ) { m_currentChannel = m_channels.begin(); }
			draw_audio_module_main( g );
			return;
		}
		if( e == Events::DIGITAL::ROTARY_L ) {
			if( m_currentChannel == m_channels.begin() ) { m_currentChannel = m_channels.end(); }
			m_currentChannel--;
			draw_audio_module_main( g );
			return;
		}
		if( e == Events::DIGITAL::BTN_ENTER ) {
			m_currentMainState++;
			draw_audio_module_sub1( g );
			return;
		}
		if( e == Events::DIGITAL::BTN_RETURN ) {
			m_currentChannel = m_channels.begin();
			m_currentMainState--;
			exit();
			return;
		}
	}
	if(m_currentMainState==m_mainStates.begin()+1) {
		if( e == Events::DIGITAL::ROTARY_R ) {
			if( m_currentEffect == m_effects.begin() ) { m_currentEffect = m_effects.end(); }
			m_currentEffect--;
			draw_audio_module_sub1( g );
			return;
		}
		if( e == Events::DIGITAL::ROTARY_L ) {
			m_currentEffect++;
			if( m_currentEffect == m_effects.end() ) { m_currentEffect = m_effects.begin(); }
			draw_audio_module_sub1( g );
			return;
		}
	}
}
void ModuleAudio::draw_audio_module_main( Graphics* g ) {
	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_top.printlnCentered( "AUDIO" );
	
	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 4 } );
	if( m_channels.begin() == m_currentChannel ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "MASTER", 0xff );
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "MASTER" );
	}
	if( m_channels.begin() + 1 == m_currentChannel ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "CHANNEL 1", 0xff );
		;
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "CHANNEL 1" );
	}
	if( m_channels.begin() + 2 == m_currentChannel ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "CHANNEL 2", 0xff );
		;
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "CHANNEL 2" );
	}
	if( m_channels.begin() + 3 == m_currentChannel ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "CHANNEL 3", 0xff );
		;
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "CHANNEL 3" );
	}
	if( m_channels.begin() + 4 == m_currentChannel ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "CHANNEL 4", 0xff );
		;
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "CHANNEL 4" );
	}
	
	g->drawWindow( m_top );
	g->drawWindow( m_bottom );
	g->display();

}

void ModuleAudio::exit()
{
	notify( *this );
}
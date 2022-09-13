#include <Arduino.h>
#include <MIDI.h>
#include <Audio.h>
#include <vector>

struct Handler
{
	uint8_t oldNote { 0 };
	std::pair <uint8_t, uint8_t> mixchannel;
};

AudioControlSGTL5000 m_audioControlSgtl5000;
AudioOutputI2S m_outputI2S;

std::pair <AudioPlaySerialflashRaw, Handler> raw0;
std::pair <AudioPlaySerialflashRaw, Handler> raw1;
std::pair <AudioPlaySerialflashRaw, Handler> raw2;
std::pair <AudioPlaySerialflashRaw, Handler> raw3;
std::pair <AudioPlaySerialflashRaw, Handler> raw4;
std::pair <AudioPlaySerialflashRaw, Handler> raw5;
std::pair <AudioPlaySerialflashRaw, Handler> raw6;
std::pair <AudioPlaySerialflashRaw, Handler> raw7;
AudioMixer4 mix00;
AudioMixer4 mix01;
AudioMixer4 mix02;
AudioMixer4 mix03;
AudioMixer4 master;

std::vector<AudioMixer4*> mixer = { &mix00, &mix01, &mix02, &mix03 };

AudioConnection p_00( raw0.first, 0, mix00, 0 );
AudioConnection p_01( raw0.first, 1, mix00, 1 );
AudioConnection p_10( raw1.first, 0, mix00, 2 );
AudioConnection p_11( raw1.first, 1, mix00, 3 );
AudioConnection p_20( raw2.first, 0, mix01, 0 );
AudioConnection p_21( raw2.first, 1, mix01, 1 );
AudioConnection p_30( raw3.first, 0, mix01, 2 );
AudioConnection p_31( raw3.first, 1, mix01, 3 );
AudioConnection p_40( raw4.first, 0, mix02, 0 );
AudioConnection p_41( raw4.first, 1, mix02, 1 );
AudioConnection p_50( raw5.first, 0, mix02, 2 );
AudioConnection p_51( raw5.first, 1, mix02, 3 );
AudioConnection p_60( raw6.first, 0, mix03, 0 );
AudioConnection p_61( raw6.first, 1, mix03, 1 );
AudioConnection p_70( raw7.first, 0, mix03, 2 );
AudioConnection p_71( raw7.first, 1, mix03, 3 );
AudioConnection p_mm0( mix00, 0, master, 0 );
AudioConnection p_mm1( mix01, 0, master, 1 );
AudioConnection p_mm2( mix02, 0, master, 2 );
AudioConnection p_mm3( mix03, 0, master, 3 );
AudioConnection p_masterl( master, 0, m_outputI2S, 0 );
AudioConnection p_masterr( master, 0, m_outputI2S, 1 );

std::vector <String> samples = { "/1.wav", "/2.wav", "/3.wav", "/4.wav", "/5.wav", "/6.wav", "/7.wav", "/8.wav", "/9.wav", "/10.wav", "/11.wav", "/12.wav", "/13.wav", "/14.wav", "/15.wav", "/62.wav", "/17.wav" };

void processNoteOn( u_int8_t channel, u_int8_t note, uint8_t velocity )
{
	if( note > samples.size())
		return;
	AudioNoInterrupts();
	if( !raw0.first.isPlaying()) {
		auto vel = static_cast<float>(velocity) / 127.0f;
		mixer[ raw0.second.mixchannel.first ]->gain( raw0.second.mixchannel.second, vel );
		raw0.first.play( samples[ note ].c_str());
		raw0.second.oldNote = note;
		AudioInterrupts();
		return;
	} else if( !raw1.first.isPlaying()) {
		auto vel = static_cast<float>(velocity) / 127.0f;
		mixer[ raw1.second.mixchannel.first ]->gain( raw1.second.mixchannel.second, vel );
		raw1.first.play( samples[ note ].c_str());
		raw1.second.oldNote = note;
		AudioInterrupts();
		return;
	} else if( !raw2.first.isPlaying()) {
		auto vel = static_cast<float>(velocity) / 127.0f;
		mixer[ raw2.second.mixchannel.first ]->gain( raw2.second.mixchannel.second, vel );
		raw2.first.play( samples[ note ].c_str());
		raw2.second.oldNote = note;
		AudioInterrupts();
		return;
	} else if( !raw3.first.isPlaying()) {
		auto vel = static_cast<float>(velocity) / 127.0f;
		mixer[ raw3.second.mixchannel.first ]->gain( raw3.second.mixchannel.second, vel );
		raw3.first.play( samples[ note ].c_str());
		raw3.second.oldNote = note;
		AudioInterrupts();
		return;
	} else if( !raw4.first.isPlaying()) {
		auto vel = static_cast<float>(velocity) / 127.0f;
		mixer[ raw4.second.mixchannel.first ]->gain( raw4.second.mixchannel.second, vel );
		raw4.first.play( samples[ note ].c_str());
		raw4.second.oldNote = note;
		AudioInterrupts();
		return;
	} else if( !raw5.first.isPlaying()) {
		auto vel = static_cast<float>(velocity) / 127.0f;
		mixer[ raw5.second.mixchannel.first ]->gain( raw5.second.mixchannel.second, vel );
		raw5.first.play( samples[ note ].c_str());
		raw5.second.oldNote = note;
		AudioInterrupts();
		return;
	} else if( !raw6.first.isPlaying()) {
		auto vel = static_cast<float>(velocity) / 127.0f;
		mixer[ raw6.second.mixchannel.first ]->gain( raw6.second.mixchannel.second, vel );
		raw6.first.play( samples[ note ].c_str());
		raw6.second.oldNote = note;
		AudioInterrupts();
		return;
	} else if( !raw7.first.isPlaying()) {
		auto vel = static_cast<float>(velocity) / 127.0f;
		mixer[ raw7.second.mixchannel.first ]->gain( raw7.second.mixchannel.second, vel );
		raw7.first.play( samples[ note ].c_str());
		raw7.second.oldNote = note;
		AudioInterrupts();
		return;
	}
	AudioInterrupts();
}

void processNoteOff( uint8_t channel, uint8_t note, uint8_t velocity )
{
	AudioNoInterrupts();
	if( raw0.first.isPlaying() && raw0.second.oldNote == note ) {
		raw0.first.stop();
	} else if( raw1.first.isPlaying() && raw1.second.oldNote == note ) {
		raw1.first.stop();
	} else if( raw2.first.isPlaying() && raw2.second.oldNote == note ) {
		raw2.first.stop();
	} else if( raw3.first.isPlaying() && raw3.second.oldNote == note ) {
		raw3.first.stop();
	} else if( raw4.first.isPlaying() && raw4.second.oldNote == note ) {
		raw4.first.stop();
	} else if( raw5.first.isPlaying() && raw5.second.oldNote == note ) {
		raw5.first.stop();
	} else if( raw6.first.isPlaying() && raw6.second.oldNote == note ) {
		raw6.first.stop();
	} else if( raw7.first.isPlaying() && raw7.second.oldNote == note ) {
		raw7.first.stop();
	}
	AudioInterrupts();
}

void setup()
{
	Serial.begin( 115200 );
	SPI.setMOSI( 11 );
	SPI.setSCK( 13 );
	
	if( !SD.begin( 10 )) {
	}
	delay( 200 );
	if( !SerialFlash.begin( 6 )) {
	}
	delay( 200 );
	
	MIDI_CREATE_DEFAULT_INSTANCE();
	MIDI.begin( MIDI_CHANNEL_OMNI );
	MIDI.setHandleNoteOn( processNoteOn );
	
	usbMIDI.setHandleNoteOn( processNoteOn );
	usbMIDI.setHandleNoteOff( processNoteOff );
	
	m_audioControlSgtl5000.enable();
	m_audioControlSgtl5000.volume( 0.2f );
	AudioMemory( 40 );
	
	raw0.second.mixchannel = { 0, 0 };
	raw1.second.mixchannel = { 0, 2 };
	raw2.second.mixchannel = { 1, 0 };
	raw3.second.mixchannel = { 1, 2 };
	raw4.second.mixchannel = { 2, 0 };
	raw5.second.mixchannel = { 2, 2 };
	raw6.second.mixchannel = { 3, 0 };
	raw7.second.mixchannel = { 3, 2 };
	
}

void loop()
{
	usbMIDI.read();
}
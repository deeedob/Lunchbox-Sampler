#include "midi_control.hpp"

MIDI_CREATE_DEFAULT_INSTANCE();

MidiControl::MidiControl() { MIDI.begin( MIDI_CHANNEL_OMNI ); }

// TODO: how do we get the length of a note and playback the sound instantly.
void MidiControl::midiReadContinuous() noexcept {
	auto get_note = []( auto& param ) {

	};
	while( true ) {
		if( usbMIDI.read() ) {
			auto type = usbMIDI.getType();
			if( type == midi::NoteOn ) {
				auto channel = usbMIDI.getChannel();
				auto data1 = usbMIDI.getData1();
				auto vel = usbMIDI.getData2();
				Note note( { type, channel, data1, vel } );
				//note send to Memory
			}
		}
	}
}
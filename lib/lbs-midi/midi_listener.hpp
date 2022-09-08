#pragma once
#include <active_object/runnable.hpp>
#include <atomic>
#include <MIDI.h>
#include <audio.hpp>
#include "note.hpp"

namespace lbs
{
	
	class MidiListener : public Runnable
	{
	public:
		/*!
		* @brief The background Midi listener.
		* Listens on Midi Input from the Host via USB and Midi DIN via 5Pin connector.
		*/
		explicit MidiListener( const std::shared_ptr<Audio>& audio )
			: m_done( false )
		{
			m_glue = this;
			MIDI_CREATE_DEFAULT_INSTANCE();
			MIDI.begin( MIDI_CHANNEL_OMNI );
			MIDI.setHandleNoteOn( processNoteOn );
			
			usbMIDI.setHandleNoteOn( processNoteOn );
			usbMIDI.setHandleNoteOff( processNoteOff );
			
			m_runnable = std::make_unique<std::thread>( &Runnable::runThread, this );
			m_audio = audio;
		}
		
		~MidiListener() override
		{
			m_done = true;
			m_runnable->join();
		}
		
		MidiListener( const MidiListener& ) = delete;
		MidiListener& operator=( const MidiListener& ) = delete;
		MidiListener( MidiListener&& ) = delete;
		MidiListener& operator=( MidiListener&& ) = delete;
		
		static void processNoteOn( u_int8_t channel, u_int8_t note, uint8_t velocity )
		{
			noInterrupts();
			auto vel = static_cast<float>(velocity) / 127.0f;
			Note n { usbMIDI.getType(), channel, note, vel, 0 };
			m_glue->m_audio->playNote( n );
			//n.printMidiData();
			interrupts();
		}
		
		static void processNoteOff( u_int8_t channel, u_int8_t note, uint8_t velocity )
		{
			noInterrupts();
			Note n { usbMIDI.getType(), channel, note, velocity };
			m_glue->m_audio->stopNote( n );
			interrupts();
		}
		
		void runTarget( void* arg ) override
		{
			while ( !m_done ) {
				usbMIDI.read();
			}
		}
	
	private:
		std::atomic<bool> m_done;
		std::unique_ptr<std::thread> m_runnable;
		std::shared_ptr<Audio> m_audio;
		static MidiListener* m_glue;
	};
}
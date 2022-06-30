#include <Arduino.h>
#include <define_t40.hpp>
#include <functional>
#include <map>
#include <MIDI.h>

/* Arduino ... why u do this shit?! */
#undef assert
#undef main

void assert_3(const char *__file, int __lineno, const char *__sexp );
#define assert( a ) if (!a) { assert_3(__FILE__, __LINE__,  #a); }

MIDI_CREATE_DEFAULT_INSTANCE();

int main() {
    MIDI.begin(MIDI_CHANNEL_OMNI);
    while(true) {
        MIDI.sendNoteOn(42, 127, 1);
        MIDI.read();
    }

    return 0;
}

// handle diagnostic informations given by assertion and abort program execution:
void assert_3(const char *__file, int __lineno, const char *__sexp) {
    // transmit diagnostic informations through serial link.
    // Serial.println(__func);
    Serial.println(" ___ ASSERT FAILED ___ FILE, LINE#, Expression ");
    Serial.println(__file);
    Serial.println(__lineno, DEC);
    Serial.println(__sexp);
    Serial.flush();
    // abort program execution.
    //xxx abort();
    Serial.println(" ___ ASSERT FAILED ___ STOPPING !!!!");
    while( 1 );
}
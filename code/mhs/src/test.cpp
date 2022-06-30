#include <Arduino.h>
#include <define_t40.hpp>
#include <functional>
#include <map>
#include <MIDIcontroller.h>
#include <MIDIUSB.h>

/* Arduino ... why u do this shit?! */
#undef assert
#undef main

void assert_3(const char *__file, int __lineno, const char *__sexp );
#define assert( a ) if (!a) { assert_3(__FILE__, __LINE__,  #a); }

/* https://github.com/joshnishikawa/MIDIcontroller/commit/f345d2e08b770a9e53a71319b5f620138afb1868
 * some changes happened? what is going on
 * */
int main() {
    Serial.begin(115200);
    while(true) {
        if(MidiUSB.available()) {
            auto msg = MidiUSB.read();
            Serial.print(msg.header);
            Serial.print(" ");
            Serial.print(msg.byte1);
            Serial.print(" ");
            Serial.print(msg.byte2);
            Serial.print(" ");
            Serial.print(msg.byte3);
            Serial.print(" ");
        }
        yield();
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
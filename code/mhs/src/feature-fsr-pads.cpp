#include <Arduino.h>
#include <define_t40.hpp>
#include <functional>
#include <map>

/* Arduino ... why u do this shit?! */
#undef assert
#undef main

void assert_3(const char *__file, int __lineno, const char *__sexp );
#define assert( a ) if (!a) { assert_3(__FILE__, __LINE__,  #a); }


/* implementation for CD74HC4051EE4 */
class FSR {
public:
    /*!
     * @param analogPin the multiplexer analog Pin
     * */
    explicit FSR(u_int8_t analogPin) {
        assert(("FSR::pin out of range", (analogPin >= 0 && analogPin <= 7)));
        std::array<bool, 3> tmp = _m_analog_lookup.find(analogPin)->second;
        m_stateController = [pos = tmp]() {
            Serial.println(pos[0]);
            Serial.println(pos[1]);
            Serial.println(pos[2]);
            if(pos[0]){
                digitalWriteFast(_FSR_SEL2, HIGH);
                Serial.println("0 1");
            }
            else {
                digitalWriteFast(_FSR_SEL2, LOW);
                Serial.println("0 0");
            }
            if(pos[1]) {
                digitalWriteFast(_FSR_SEL1, HIGH);
                Serial.println("1 1");
            }
            else {
                digitalWriteFast(_FSR_SEL1, LOW);
                Serial.println("1 0");
            }
            if(pos[2]) {
                Serial.println("2 1");
                digitalWriteFast(_FSR_SEL0, HIGH);
            }
            else {
                Serial.println("2 0");
                digitalWriteFast(_FSR_SEL0, LOW);
            }
        };
    }
    void setActive() {
        m_stateController();
    }
private:
    std::function<void()> m_stateController;
    bool m_isActive;
    /* map of <mltpxAPin, <sig0, sig1, sig2>> -> A0 - A7 */
    /* from the multiplexer docs */
    std::map<u_int8_t , std::array<bool, 3>> _m_analog_lookup = {
            {0, {false, false, false}},
            {1, {false, false, true }},
            {2, {false, true, false }},
            {3, {false, true, true  }},
            {4, {true, false, false }},
            {5, {true, false, true  }},
            {6, {true, true, false  }},
            {7, {true, true, true   }},
    };


};

int main() {
    Serial.begin(9600);
    Serial.println(":::Beginning Multiplexer Test:::");
    pinMode(_FSR_SEL0, OUTPUT);
    pinMode(_FSR_SEL1, OUTPUT);
    pinMode(_FSR_SEL2, OUTPUT);
    digitalWrite(_FSR_SEL0, HIGH);
    digitalWrite(_FSR_SEL1, HIGH);
    digitalWrite(_FSR_SEL2, HIGH);
    FSR testPad(7);
    testPad.setActive();

    while(true) {
        testPad.setActive();
        Serial.print("MLTPX : ");
        Serial.println(analogRead(_FSR_POLL));
        delay(500);
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
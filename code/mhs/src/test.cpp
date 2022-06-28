#include <Arduino.h>
#include <define_t40.hpp>
#include <functional>
#include <frozen/map.h>
#include <Bounce2.h>

/* Arduino ... why u do this shit?! */
#undef main

/* implementation for CD74HC4051EE4 */
class FSR {
public:
    /*!
     * @param analogPin the multiplexer analog Pin
     * */
    explicit FSR(u_int8_t analogPin) {
        assert(("analogPin is out of Range!", analogPin >= 0 && analogPin <= 7));
        std::array<bool, 3> tmp = m_analog_lookup.find(analogPin)->second;
        m_stateController = [&tmp]() {
            if(tmp[0])
                digitalWriteFast(_FSR_SEL0, HIGH);
            else
                digitalWriteFast(_FSR_SEL0, LOW);
            if(tmp[1])
                digitalWriteFast(_FSR_SEL1, HIGH);
            else
                digitalWriteFast(_FSR_SEL1, LOW);
            if(tmp[2])
                digitalWriteFast(_FSR_SEL2, HIGH);
            else
                digitalWriteFast(_FSR_SEL2, LOW);
        };
    }
    void setActive() {
        Serial.println("setActive()");
        m_stateController();
    }
private:
    std::function<void()> m_stateController;
    /* map of <mltpxAPin, <sig0, sig1, sig2>> -> A0 - A7 */
    /* from the multiplexer docs */
    static constexpr frozen::map<u_int8_t , std::array<bool, 3>, 8> m_analog_lookup = {
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
    FSR testPad(4);
    testPad.setActive();
    Bounce bounce(_BTN_TOGGLE, 20);

    while(true) {
        bounce.update();
        if(bounce.fell()) {
            Serial.println("HI");
        }
        delay(300);
    }

    return 0;
}
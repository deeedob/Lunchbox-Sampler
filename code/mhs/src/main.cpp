#include <define_t40.hpp>
#include <interrupt_system/analog_interrupts.hpp>
#include <interrupt_system/digital_interrupts.hpp>
#include <event_sytem.hpp>

using namespace lbs;
int main() {
    Serial.begin(9600);
    while(!Serial) {
        yield();
    }
    Serial.println(":::Begin Event System Test:::");
    auto eventSystem = std::make_shared<EventSystem>();
    DigitalInterrupts dig_int(eventSystem);
    AnalogInterrupts an_int(eventSystem);
}
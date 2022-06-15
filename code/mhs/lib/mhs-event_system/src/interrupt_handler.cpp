#include "interrupt_handler.hpp"
namespace mhs {

/* _________________________ANALOG INTERRUPTS______________________ */
/* ----------------------------------------------------------------- */
/* _________________________________________________________________ */

    InterruptHandler::AnalogInterrupts::AnalogInterrupts() {
        /* TODO: add FSR */
        m_analog_lookup = {
            { EventSystem::Events::ANALOG::POT0, _POT0 },
            { EventSystem::Events::ANALOG::POT1, _POT1 },
            { EventSystem::Events::ANALOG::POT2, _POT2 },
            { EventSystem::Events::ANALOG::POT3, _POT3 },
        };
        setup_all_isr();
    }

    InterruptHandler::AnalogInterrupts::~AnalogInterrupts() {
        for(auto& i : m_analog_lookup) {
            disablePin(i.first);
        }
    }

    void InterruptHandler::AnalogInterrupts::setup_all_isr() {

    }

    void InterruptHandler::AnalogInterrupts::enablePin( EventSystem::Events::ANALOG e, void (*function)( ), int mode ) {
        attachInterrupt(m_analog_lookup.find(e)->second, function, mode);
    }

    void InterruptHandler::AnalogInterrupts::disablePin( EventSystem::Events::ANALOG e ) {
        detachInterrupt(m_analog_lookup.find(e)->second);
    }

    void InterruptHandler::AnalogInterrupts::readAllOldValues() {
        for( auto& i : _pinAnalogOldPot().load()){
            i = start
        }
    }


/* _________________________DIGITAL INTERRUPTS______________________ */
/* ----------------------------------------------------------------- */
/* _________________________________________________________________ */

    InterruptHandler::DigitalInterrupts::DigitalInterrupts() {
        m_dig_lookup = {
                { EventSystem::Events::DIGITAL::ROTARY_L, _ROTARY_A },
                { EventSystem::Events::DIGITAL::ROTARY_R, _ROTARY_B },
                { EventSystem::Events::DIGITAL::BTN_ENTER, _BTN_ENTER },
                { EventSystem::Events::DIGITAL::BTN_RETURN, _BTN_RETURN },
                { EventSystem::Events::DIGITAL::BTN_TOGGLE, _BTN_TOGGLE },
                { EventSystem::Events::DIGITAL::MIDI_IN, _MIDI_IN },
        };
    }

    InterruptHandler::DigitalInterrupts::DigitalInterrupts(
            std::initializer_list<std::pair<EventSystem::Events::DIGITAL, u_int8_t>> list ) {
        /* TODO add dynamic initialization!*/
    }
    /*
     * Disable all pins if class gets out of scope
     * */
    InterruptHandler::DigitalInterrupts::~DigitalInterrupts() {
        for(auto& i : m_dig_lookup) {
            disablePin(i.first);
        }
    }

    void InterruptHandler::DigitalInterrupts::setup_all_isr() {
        /* Setup the rotary interrupts for left and right direction!
         * We are using 2 variables declared in global mhs...
         * */
        attachInterrupt(digitalPinToInterrupt(_ROTARY_B), [](){
            _pinAstateCurrent = digitalReadFast(_ROTARY_A);
            if((_pinAstateLast == LOW) && (_pinAstateCurrent == HIGH)) {
                if ( digitalReadFast(_ROTARY_B) == HIGH ) {
                    m_event_system.enqueueDigital(EventSystem::Events::DIGITAL::ROTARY_L);
                } else {
                    m_event_system.enqueueDigital(EventSystem::Events::DIGITAL::ROTARY_R);
                }
            }
            } , CHANGE);

        attachInterrupt(digitalPinToInterrupt(_BTN_ENTER),[](){
            m_event_system.enqueueDigital(EventSystem::Events::DIGITAL::BTN_ENTER);
        }, CHANGE);


        attachInterrupt(digitalPinToInterrupt(_BTN_RETURN),[](){
            m_event_system.enqueueDigital(EventSystem::Events::DIGITAL::BTN_RETURN);
        }, CHANGE);

        attachInterrupt(digitalPinToInterrupt(_BTN_TOGGLE),[](){
            m_event_system.enqueueDigital(EventSystem::Events::DIGITAL::BTN_TOGGLE);
        }, CHANGE);
    }

    void InterruptHandler::DigitalInterrupts::enablePin(mhs::EventSystem::Events::DIGITAL e, void (*function)(), int mode) {
        attachInterrupt(m_dig_lookup.find(e)->second, function, mode);
    }

    void InterruptHandler::DigitalInterrupts::disablePin(mhs::EventSystem::Events::DIGITAL e) {
        detachInterrupt(m_dig_lookup.find(e)->second);
    }

}

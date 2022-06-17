#include "interrupt_handler.hpp"
namespace mhs {

/* _________________________ANALOG INTERRUPTS______________________ */
/* ----------------------------------------------------------------- */
/* _________________________________________________________________ */
    InterruptHandler::AnalogInterrupts::AnalogInterrupts(u_int16_t _delta)
        : delta(_delta)
    {
        /* TODO: add FSR, in separate maps?! */
        readAllOldValues();
        enableAll();
    }

    InterruptHandler::AnalogInterrupts::~AnalogInterrupts() {
        disableAll();
    }

    /* Audio library uses ADC0 so try to use ADC1*/
    void InterruptHandler::AnalogInterrupts::readAllOldValues() {
        u_int8_t pos = 0;
        u_int8_t pin;
        ADC_STATE state;

        for(const auto & it : m_analog_lookup) {
            pos = static_cast<u_int8_t>(it.first);
            pin = it.second.first;
            state = it.second.second;
            if(state == ADC_STATE::ADC1 || state == ADC_STATE::BOTH) {
                adc.adc1->startSingleRead(pin);
                _pinAnalogPotOldValue().load()[pos] = adc.adc1->readSingle();
#ifdef VERBOSE
                Serial.print("POT: "); Serial.print(pos); Serial.print(" VAL: "); Serial.println(_pinAnalogPotOldValue().load()[pos])
#endif
            } else {
                /* ADC_STATE::ADC0*/
                adc.adc0->startSingleRead(pin);
                _pinAnalogPotOldValue().load()[pos] = adc.adc0->readSingle();
#ifdef VERBOSE
                Serial.print("POT: "); Serial.print(pos); Serial.print(" VAL: "); Serial.println(_pinAnalogPotOldValue().load()[pos])
#endif
            }
        }
    }

    void InterruptHandler::AnalogInterrupts::enableAll() {
        enableADC0();
        enableADC1();
    }

    void InterruptHandler::AnalogInterrupts::enableADC0() {
        adc.adc0->enableInterrupts([](){
            _pinAnalogPotOldValue().load()[_analogReadPosition] = adc.adc0->analogReadContinuous();
        });
    }

    void InterruptHandler::AnalogInterrupts::enableADC1() {
        adc.adc1->enableInterrupts([](){
            _pinAnalogPotOldValue().load()[_analogReadPosition] = adc.adc1->analogReadContinuous();
        });
    }

    void InterruptHandler::AnalogInterrupts::disableAll() {
        disableADC0();
        disableADC1();
    }

    void InterruptHandler::AnalogInterrupts::disableADC0() {
        adc.adc0->disableInterrupts();
    }

    void InterruptHandler::AnalogInterrupts::disableADC1() {
        adc.adc1->disableInterrupts();
    }

    void InterruptHandler::AnalogInterrupts::readSinglePot0() const {
        constexpr auto pos = static_cast<u_int8_t>(EventSystem::Events::ANALOG::POT0);
        constexpr auto pin = m_analog_lookup.find(EventSystem::Events::ANALOG::POT0)->second.first;
        constexpr auto adc_mode = m_analog_lookup.find(EventSystem::Events::ANALOG::POT0)->second.second;
        if (adc_mode == ADC_STATE::ADC1 || adc_mode == ADC_STATE::BOTH) {
            adc.adc1->startContinuous(pin);
        }
    }

    void InterruptHandler::AnalogInterrupts::readSinglePot1() const {
        constexpr auto pos = static_cast<u_int8_t>(EventSystem::Events::ANALOG::POT1);
        constexpr auto pin = m_analog_lookup.find(EventSystem::Events::ANALOG::POT1)->second.first;
        constexpr auto adc_mode = m_analog_lookup.find(EventSystem::Events::ANALOG::POT1)->second.second;
        if (adc_mode == ADC_STATE::ADC1 || adc_mode == ADC_STATE::BOTH) {
            adc.adc1->startContinuous(pin);
        }
    }

    void InterruptHandler::AnalogInterrupts::readSinglePot2() const {

    }

    void InterruptHandler::AnalogInterrupts::readSinglePot3() const {

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

    InterruptHandler::InterruptHandler()
    {
    }
}

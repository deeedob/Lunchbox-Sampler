#include "digital_interrupts.hpp"

using namespace lbs;
/* todo make singleton?*/
DigitalInterrupts * DigitalInterrupts::instance = nullptr;

DigitalInterrupts::DigitalInterrupts(const std::shared_ptr<EventSystem>& eventSystem, u_int8_t bounceTime)
    : m_btnEnter(BTN_ENTER_, bounceTime), m_btnReturn(BTN_RETURN_, bounceTime),
      m_btnToggle(BTN_TOGGLE_, bounceTime)
{
    m_eventSystem = eventSystem;
    instance = this;
    glob_rotaryCurrent = LOW;
    glob_rotaryLast = LOW;
};

/* Disable all pins if class gets out of scope */
DigitalInterrupts::~DigitalInterrupts() {
    disableAll();
}

void DigitalInterrupts::enablePin( Events::DIGITAL e ) {
    auto tmp = getTable().find(e)->second;
    attachInterrupt(tmp.first, tmp.second , CHANGE);
}

void DigitalInterrupts::enableCustomizedPin( Events::DIGITAL e, void (*function)(), int mode ) {
    attachInterrupt(getTable().find(e)->second.first, function, mode);
}

void DigitalInterrupts::enableAll() {
    for(auto& i : getTable()) {
        enablePin(i.first);
    }
}

void DigitalInterrupts::disablePin( Events::DIGITAL e ) {
    detachInterrupt(getTable().find(e)->second.first);
}

const DigitalInterrupts::dig_lookup &DigitalInterrupts::getTable() {
    static const auto* table = new dig_lookup({
        { Events::DIGITAL::ROTARY_R,    { ROTARY_B_,    isr_rotary } },
        { Events::DIGITAL::BTN_ENTER,   { BTN_ENTER_,   isr_btn_enter } },
        { Events::DIGITAL::BTN_RETURN,  { BTN_RETURN_,  isr_btn_return } },
        { Events::DIGITAL::BTN_TOGGLE,  { BTN_TOGGLE_,  isr_btn_toggle } }
    });
    return *table;
}

void DigitalInterrupts::disableAll() {
    for(auto& i : getTable()) {
        disablePin(i.first);
    }
}

void DigitalInterrupts::isr_rotary() {
    DigitalInterrupts::instance->glob_rotaryCurrent = digitalReadFast(ROTARY_A_);
    if((DigitalInterrupts::instance->glob_rotaryLast == LOW ) && (DigitalInterrupts::instance->glob_rotaryCurrent == HIGH )) {
        if( digitalReadFast(ROTARY_B_) == HIGH ) {
#ifdef VERBOSE
    Serial.print("ISR::ROTARY:: "); Serial.print(ROTARY_B_); Serial.println(" LEFT");
#endif
            DigitalInterrupts::instance->m_eventSystem->enqueueDigital(Events::DIGITAL::ROTARY_L);
        } else {
#ifdef VERBOSE
            Serial.print("ISR::ROTARY:: "); Serial.print(ROTARY_B_); Serial.println(" RIGHT");
#endif
            DigitalInterrupts::instance->m_eventSystem->enqueueDigital(Events::DIGITAL::ROTARY_R);
        }
    }
}

void DigitalInterrupts::isr_btn_enter() {
    DigitalInterrupts::instance->m_btnEnter.update();
    /* on button enter */
    if(DigitalInterrupts::instance->m_btnEnter.fallingEdge()) {
#ifdef VERBOSE
        Serial.print("ISR::BTN:: ");
        Serial.print(BTN_ENTER_);
        Serial.println(" ENTER");
#endif
        DigitalInterrupts::instance->m_eventSystem->enqueueDigital(
                Events::DIGITAL::BTN_ENTER);
    }
}

void DigitalInterrupts::isr_btn_return() {
    DigitalInterrupts::instance->m_btnReturn.update();
    /* on button enter */
    if(DigitalInterrupts::instance->m_btnReturn.fallingEdge()) {
#ifdef VERBOSE
        Serial.print("ISR::BTN:: ");
        Serial.print(BTN_RETURN_);
        Serial.println(" RETURN");
#endif
        DigitalInterrupts::instance->m_eventSystem->enqueueDigital(
                Events::DIGITAL::BTN_RETURN);
    }
}

void DigitalInterrupts::isr_btn_toggle() {
    DigitalInterrupts::instance->m_btnToggle.update();
    /* on button enter */
    if(DigitalInterrupts::instance->m_btnToggle.fallingEdge()) {
#ifdef VERBOSE
        Serial.print("ISR::BTN:: "); Serial.print(BTN_TOGGLE_); Serial.println(" TOGGLE");
#endif
        DigitalInterrupts::instance->m_eventSystem->enqueueDigital(Events::DIGITAL::BTN_TOGGLE);
    }
}

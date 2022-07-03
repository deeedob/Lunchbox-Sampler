#include "digital_interrupts.hpp"

using namespace lbs;

DigitalInterrupts::DigitalInterrupts(const std::shared_ptr<EventSystem>& eventSystem) {
    m_eventSystem = eventSystem;
    instance = this;
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
        { Events::DIGITAL::ROTARY_R, {_ROTARY_B, isr_rotary } },
        { Events::DIGITAL::BTN_ENTER, {_BTN_ENTER, isr_btn_enter } },
        { Events::DIGITAL::BTN_RETURN, {_BTN_RETURN, isr_btn_return } },
        { Events::DIGITAL::BTN_TOGGLE, {_BTN_TOGGLE, isr_btn_toggle } }
    });
    return *table;
}

void DigitalInterrupts::disableAll() {
    for(auto& i : getTable()) {
        disablePin(i.first);
    }
}

void DigitalInterrupts::isr_rotary() {
    _glob_rotaryCurrent = digitalReadFast(_ROTARY_A);
    if(( _glob_rotaryLast == LOW ) && ( _glob_rotaryCurrent == HIGH )) {
        if( digitalReadFast(_ROTARY_B) == HIGH ) {
            DigitalInterrupts::instance->m_eventSystem->enqueueDigital(Events::DIGITAL::ROTARY_L);
        } else {
            DigitalInterrupts::instance->m_eventSystem->enqueueDigital(Events::DIGITAL::ROTARY_R);
        }
    }
}

void DigitalInterrupts::isr_btn_enter() {
    DigitalInterrupts::instance->m_eventSystem->enqueueDigital(Events::DIGITAL::BTN_ENTER);
}

void DigitalInterrupts::isr_btn_return() {
    DigitalInterrupts::instance->m_eventSystem->enqueueDigital(Events::DIGITAL::BTN_RETURN);
}

void DigitalInterrupts::isr_btn_toggle() {
    DigitalInterrupts::instance->m_eventSystem->enqueueDigital(Events::DIGITAL::BTN_TOGGLE);
}



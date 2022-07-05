#include "digital_interrupts.hpp"

using namespace lbs;
DigitalInterrupts * DigitalInterrupts::instance = nullptr;

DigitalInterrupts::DigitalInterrupts(const std::shared_ptr<EventSystem>& eventSystem, u_int8_t bounceTime)
    : m_btnEnter(BTN_ENTER_, bounceTime), m_btnReturn(BTN_RETURN_, bounceTime),
      m_btnToggle(BTN_TOGGLE_, bounceTime)
{
    m_eventSystem = eventSystem;
    instance = this;
};

/* Disable all pins if class gets out of scope */
DigitalInterrupts::~DigitalInterrupts() {
    disableAll();
}

void DigitalInterrupts::enableAll() {
    for(auto& i : getTable()) {
        attachInterrupt(std::get<0>(i.second), std::get<1>(i.second) , std::get<2>(i.second));
    }
}

void DigitalInterrupts::enablePin( Events::DIGITAL e ) {
    auto tmp = getTable().find(e)->second;
    attachInterrupt(std::get<0>(tmp), std::get<1>(tmp) , std::get<2>(tmp));
}

void DigitalInterrupts::enableCustomizedPin( Events::DIGITAL e, void (*function)(), int mode ) {
    auto tmp = getTable().find(e)->second;
    attachInterrupt(std::get<0>(tmp), function, mode);
}

void DigitalInterrupts::disablePin( Events::DIGITAL e ) {
    auto tmp = getTable().find(e)->second;
    detachInterrupt(std::get<0>(tmp));
}

const DigitalInterrupts::dig_lookup &DigitalInterrupts::getTable() {
    /* Events :: {Pin :: ISR :: Mode } */
    static const auto* table = new dig_lookup({
          //{ Events::DIGITAL::ROTARY_R,   std::make_tuple( ROTARY_B_     , isr_rotaryB    , LOW     )},
          { Events::DIGITAL::ROTARY,   std::make_tuple( ROTARY_A_     , isr_rotaryA    , LOW     )},
          { Events::DIGITAL::BTN_ENTER,  std::make_tuple( BTN_ENTER_    , isr_btn_enter  , CHANGE  )},
          { Events::DIGITAL::BTN_RETURN, std::make_tuple( BTN_RETURN_   , isr_btn_return , CHANGE  )},
          { Events::DIGITAL::BTN_TOGGLE, std::make_tuple( BTN_TOGGLE_   , isr_btn_toggle , CHANGE  )}
    });
    return *table;
}

void DigitalInterrupts::disableAll() {
    for(auto& i : getTable()) {
        detachInterrupt(std::get<0>(i.second));
    }
}

void DigitalInterrupts::isr_rotaryA() {
    static unsigned long lastInterruptTime = 0;
    unsigned long interruptTime = millis();
    /* if the interrupt comes faster then 5ms assume it's a bounce */
    if ( interruptTime - lastInterruptTime > 5 ) {
        if ( digitalRead(ROTARY_B_) == HIGH ) {
#ifdef VERBOSE
            Serial.print("ISR::ROTARY:: "); Serial.print(ROTARY_B_);Serial.println(" LEFT");
#endif
        }
        else {
#ifdef VERBOSE
            Serial.print("ISR::ROTARY:: "); Serial.print(ROTARY_B_);Serial.println(" RIGHT");
#endif
        }
    }
    lastInterruptTime = interruptTime;
}

void DigitalInterrupts::isr_rotaryB() {
    static unsigned long lastInterruptTime = 0;
    unsigned long interruptTime = millis();
    /* if the interrupt comes faster then 5ms assume it's a bounce */
    if ( interruptTime - lastInterruptTime > 20 ) {
    }
    lastInterruptTime = interruptTime;
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
        DigitalInterrupts::instance->m_eventSystem->enqueueDigital(Events::DIGITAL::BTN_ENTER);
    }
}

void DigitalInterrupts::isr_btn_return() {
    DigitalInterrupts::instance->m_btnReturn.update();
    /* on button enter */
    if(DigitalInterrupts::instance->m_btnReturn.fallingEdge()) {
#ifdef VERBOSE
        Serial.print("ISR::BTN:: "); Serial.print(BTN_RETURN_); Serial.println(" RETURN");
#endif
        DigitalInterrupts::instance->m_eventSystem->enqueueDigital(Events::DIGITAL::BTN_RETURN);
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

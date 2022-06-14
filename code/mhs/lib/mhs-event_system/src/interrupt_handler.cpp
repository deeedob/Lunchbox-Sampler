#include "interrupt_handler.hpp"
namespace mhs {

/* _________________________ANALOG INTERRUPTS______________________ */
mhs::InterruptHandler::AnalogInterrupts::AnalogInterrupts( std::initializer_list<EventSystem>&& mapping )
{
}

mhs::InterruptHandler::AnalogInterrupts::~AnalogInterrupts() {

}

void mhs::InterruptHandler::AnalogInterrupts::enablePin(unsigned short pin ) {

}

void mhs::InterruptHandler::AnalogInterrupts::disablePin(unsigned short pin ) {

}

/* _________________________DIGITAL INTERRUPTS______________________ */

mhs::InterruptHandler::DigitalInterrupts::DigitalInterrupts(std::initializer_list<EventSystem::EventInfo> mapping ) {
}

mhs::InterruptHandler::DigitalInterrupts::~DigitalInterrupts() {
}

void mhs::InterruptHandler::DigitalInterrupts::setup_isr(EventSystem::EventInfo event_dig ) {
    /* Setup the rotary interrupts for left and right direction!
     * We are using 2 variables declared in global mhs...
     * */
    attachInterrupt(digitalPinToInterrupt(ROTARY_B), [](){
        __pinAstateCurrent = digitalReadFast(ROTARY_A);
        if((__pinAstateLast == LOW) && (__pinAstateCurrent == HIGH)) {
            if ( digitalReadFast(ROTARY_B) == HIGH ) {
                m_eventSystem.enqueueDigital(EventSystem::Events::DIGITAL::ROTARY::LEFT);
            } else {
                m_eventSystem.enqueueDigital(EventSystem::Events::DIGITAL::ROTARY::RIGHT);
            }
        }
        } , CHANGE);
}

void mhs::InterruptHandler::DigitalInterrupts::enablePin(mhs::EventSystem::Events e) {

}

void mhs::InterruptHandler::DigitalInterrupts::disablePin(mhs::EventSystem::Events e) {

}

void mhs::InterruptHandler::DigitalInterrupts::fp_wrapper() {
    m_eventSystem.enqueueDigital(_current);
}

}

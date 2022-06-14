#include "interrupt_handler.hpp"

/* _________________________ANALOG INTERRUPTS______________________ */
mhs::InterruptHandler::AnalogInterrupts::AnalogInterrupts() {

}

mhs::InterruptHandler::AnalogInterrupts::~AnalogInterrupts() {

}


void mhs::InterruptHandler::AnalogInterrupts::enableAll() {

}

void mhs::InterruptHandler::AnalogInterrupts::disableAll() {

}

void mhs::InterruptHandler::AnalogInterrupts::enablePin(unsigned short pin ) {

}

void mhs::InterruptHandler::AnalogInterrupts::disablePin(unsigned short pin ) {

}

void mhs::InterruptHandler::AnalogInterrupts::setup_isr() {

}

/* _________________________DIGITAL INTERRUPTS______________________ */

mhs::InterruptHandler::DigitalInterrupts::DigitalInterrupts() {
    setup_isr();
}

mhs::InterruptHandler::DigitalInterrupts::~DigitalInterrupts() {
    disableAll();
}

void mhs::InterruptHandler::DigitalInterrupts::enableAll() {

}

void mhs::InterruptHandler::DigitalInterrupts::disableAll() {

}

void mhs::InterruptHandler::DigitalInterrupts::enablePin(unsigned short pin ) {

}

void mhs::InterruptHandler::DigitalInterrupts::disablePin(unsigned short pin ) {

}

void mhs::InterruptHandler::DigitalInterrupts::setup_isr() {
    attachInterrupt(digitalPinToInterrupt(ROTARY_SW), []() {
    }, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROTARY_SW), []() {
    }, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROTARY_SW), []() {
    }, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROTARY_SW), []() {
    }, CHANGE);
}


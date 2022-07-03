#pragma once
#include <map>
#include <Arduino.h>
#include "events.hpp"
#include "define_t40.hpp"
#include "event_sytem.hpp"

namespace lbs {
    volatile int _glob_rotaryCurrent = LOW;
    volatile int _glob_rotaryLast = LOW;
    class DigitalInterrupts
    {
        using dig_lookup = std::map<Events::DIGITAL, std::pair<u_int8_t, void (*)()>>;
    public:
        explicit DigitalInterrupts(const std::shared_ptr<EventSystem>& eventSystem);
        ~DigitalInterrupts();
        void enablePin(Events::DIGITAL e);
        void enableCustomizedPin( Events::DIGITAL e, void (*function)(), int mode = CHANGE );
        void enableAll();

        void disablePin(Events::DIGITAL e);
        void disableAll();
        static const dig_lookup& getTable();

        /* isr functions */
        static void isr_rotary();
        static void isr_btn_enter();
        static void isr_btn_return();
        static void isr_btn_toggle();
    private:
        std::shared_ptr<EventSystem> m_eventSystem;
        static DigitalInterrupts* instance;
    };
}

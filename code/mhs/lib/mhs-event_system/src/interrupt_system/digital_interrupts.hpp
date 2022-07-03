#pragma once
#include <map>
#include <Arduino.h>
#include <Bounce.h>
#include <memory>
#include "events.hpp"
#include "define_t40.hpp"
#include "event_sytem.hpp"


namespace lbs {
    class DigitalInterrupts
    {
        using dig_lookup = std::map<Events::DIGITAL, std::pair<u_int8_t, void (*)()>>;
    public:
        explicit DigitalInterrupts(const std::shared_ptr<EventSystem>& eventSystem, u_int8_t bounceTime = 20);
        ~DigitalInterrupts();
        void enablePin(Events::DIGITAL e);
        void enableCustomizedPin( Events::DIGITAL e, void (*function)(), int mode = CHANGE );
        void enableAll();

        void disablePin(Events::DIGITAL e);
        void disableAll();
        static const dig_lookup& getTable();
        static DigitalInterrupts& getInstance();

        /* isr functions */
        static void isr_rotary();
        static void isr_btn_enter();
        static void isr_btn_return();
        static void isr_btn_toggle();
    private:
        std::shared_ptr<EventSystem> m_eventSystem;
        volatile int glob_rotaryCurrent;
        volatile int glob_rotaryLast;
        Bounce m_btnEnter;
        Bounce m_btnReturn;
        Bounce m_btnToggle;
        /* glue routine to have a class like feeling in isr */
        static DigitalInterrupts* instance;
    };
}

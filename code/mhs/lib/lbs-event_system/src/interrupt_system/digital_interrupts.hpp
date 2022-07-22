#pragma once
#include <map>
#include <Arduino.h>
#include <Bounce.h>
#include <Encoder.h>
#include <memory>
#include "events.hpp"
#include "define_t40.hpp"
#include "event_sytem.hpp"


namespace lbs {

    class DigitalInterrupts
    {
        using dig_lookup = std::map<Events::DIGITAL, std::tuple<u_int8_t, void (*)(), u_int8_t>>;
    public:
        explicit DigitalInterrupts(const std::shared_ptr<EventSystem>& eventSystem, u_int8_t bounceTime = 5);
        ~DigitalInterrupts();
        void enablePin(Events::DIGITAL e);
        void enableCustomizedPin( Events::DIGITAL e, void (*function)(), int mode = CHANGE );
        void enableAll();

        void disablePin(Events::DIGITAL e);
        void disableAll();
        static const dig_lookup& getTable();

        /* isr functions */
        static void isr_rotaryA();
        static void isr_rotaryB();
        static void isr_btn_enter();
        static void isr_btn_return();
        static void isr_btn_toggle();
    private:
        std::shared_ptr<EventSystem> m_eventSystem;
        Bounce m_btnEnter;
        Bounce m_btnReturn;
        Bounce m_btnToggle;
        /* glue routine to have a class like feeling in isr */
        static DigitalInterrupts* instance;
    };
}

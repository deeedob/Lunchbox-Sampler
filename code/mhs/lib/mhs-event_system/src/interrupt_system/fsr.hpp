#pragma once
#include "../include/define_t40.hpp"
#include "multiplex.hpp"
#include "event_sytem.hpp"
#include "interrupt_handler.hpp"
#include <atomic>

namespace lbs {

    class FSR : public Multiplex
    {
    public:
        using fsr_lookup = std::pair<EventSystem::Events::ANALOG, std::pair<u_int8_t, InterruptHandler::ADC_STATE>>;

        explicit FSR(u_int8_t mpxPin0, u_int8_t mpxPin1, u_int8_t mpxPin2, u_int8_t mpxPin3, u_int16_t delta );
        u_int8_t next();

        u_int16_t getDelta() const;
        void setDelta( u_int16_t mDelta );

    private:
        std::atomic<u_int8_t> m_pos;
        std::atomic<std::array<u_int16_t, 4>> m_values;
        u_int16_t m_delta;


        std::array<Multiplex, 4> m_pads;
    };
}

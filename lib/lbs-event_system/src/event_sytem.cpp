#include "event_sytem.hpp"

lbs::EventSystem::EventSystem() {
    /* we can't call nullptr in the consumer so init to real void func! */
    for( int i = static_cast<int>(Events::DIGITAL::ROTARY_L); static_cast<Events::DIGITAL>(i) < Events::DIGITAL::LAST; i++) {
        m_digMapping[static_cast<Events::DIGITAL>(i)] = [](){ };
    }
    for( int i = static_cast<int>(Events::Analog::POTS::POT0); static_cast<Events::Analog::POTS>(i) < Events::Analog::POTS::POT3; i++) {
        m_analogMapping[static_cast<Events::Analog::POTS>(i)] = [](u_int16_t){ };
    }
}

void lbs::EventSystem::enqueueDigital( Events::DIGITAL e ) {
    scheduler.m_digitalListener.send(m_digMapping.find(e)->second, nullptr);
}

void lbs::EventSystem::enqueueAnalog( Events::Analog::POTS e, u_int16_t value ) {
    //scheduler.m_analogListener.send(m_analogMapping.find(e)->second);
    auto f = m_analogMapping.find(e)->second;
    //scheduler.m_analogListener.send(f, value);
}

void lbs::EventSystem::attachDigital( lbs::Events::DIGITAL e, std::function<void()> f ) {
    m_digMapping[e] = std::move(f);
}

void lbs::EventSystem::detachDigital( lbs::Events::DIGITAL e ) {
    m_digMapping[e] = [](){};
}

void lbs::EventSystem::attachAnalog( lbs::Events::Analog::POTS e, std::function<void(u_int16_t )> f ) {
    m_analogMapping[e] = std::move(f);
}

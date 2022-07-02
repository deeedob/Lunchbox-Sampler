#include "event_sytem.hpp"

void lbs::EventSystem::enqueueDigital( Events::DIGITAL e ) {
    scheduler.m_digitalListener.send(m_digMapping.find(e)->second);
}

void lbs::EventSystem::enqueueAnalog( Events::ANALOG e ) {
    scheduler.m_analogListener.send(m_analogMapping.find(e)->second);
}

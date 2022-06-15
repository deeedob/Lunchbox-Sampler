#include "event_sytem.hpp"

void mhs::EventSystem::enqueueDigital( Events::DIGITAL e ) {
    scheduler.m_digitalListener.send(m_digMapping.find(e)->second);
}

void mhs::EventSystem::enqueueAnalog( Events::ANALOG e ) {
    scheduler.m_analogListener.send(m_analogMapping.find(e)->second);
}

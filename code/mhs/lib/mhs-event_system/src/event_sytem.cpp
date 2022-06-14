#include "event_sytem.hpp"

void mhs::EventSystem::enqueueDigital( Events e ) {
    scheduler.m_digitalListener.send(eventTable.find(e)->second);
}

void mhs::EventSystem::enqueueAnalog( Events e ) {
    scheduler.m_analogListener.send(eventTable.find(e)->second);
}

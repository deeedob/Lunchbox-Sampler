#pragma once
#include "event_sytem.hpp"

namespace mhs
{
    class AbstractInterrupt
    {
    private:
        virtual void enablePin( EventSystem::Events e ) = 0;
        virtual void disablePin( EventSystem::Events e ) = 0;
        /* An interface SHOULD have a virtual destructor!
         * We can't create an instance of our interface class, but we
         * can delete the interface pointer to this interface!
         * */
        virtual ~AbstractInterrupt() { };
    };
}
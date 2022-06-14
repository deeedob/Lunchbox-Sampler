#pragma once

class AbstractInterrupt {
public:
    virtual void enableAll() = 0;
    virtual void disableAll() = 0;
    virtual void enablePin(unsigned short pin) = 0;
    virtual void disablePin(unsigned short pin) = 0;
    virtual void setup_isr() = 0;
    /* An interface SHOULD have a virtual destructor!
     * We can't create an instance of our interface class, but we
     * can delete the interface pointer to this interface!
     * */
    virtual ~AbstractInterrupt() { };
};
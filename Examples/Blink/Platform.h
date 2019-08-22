#ifndef PLATFORM_H
#define PLATFORM_H

enum PinMode
{
    PIN_INPUT_PULLUP,
    PIN_OUTPUT
};


/**
*   Interface for users of Platforms
*   This interface abstracts the application from the underlying platform
*/

class Platform
{
public:
    Platform(){}
    virtual ~Platform(){}

    virtual unsigned long getSystemUpTimeMicros() = 0;
    virtual unsigned long getSystemUpTimeMillis() = 0;
    virtual unsigned long getSystemUpTimeMinutes() = 0;
    virtual void setPinMode(int pin, PinMode mode) = 0;
    virtual void setPin(int pin, bool level) = 0;
    virtual bool getPin(int pin) = 0;
    virtual int readAnalogPin(int pin) = 0;
    virtual void delay(unsigned millis) = 0;
    virtual long map(long x, long in_min, long in_max, long out_min, long out_max) = 0;
};

#endif // PLATFORM_H

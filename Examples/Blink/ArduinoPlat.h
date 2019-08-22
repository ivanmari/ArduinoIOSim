#ifndef ARDUINOPLAT_H
#define ARDUINOPLAT_H

#ifndef X86_PLAT

#include "Platform.h"

/**
*   Platform implementation for Arduino
*
*/

class ArduinoPlat : public Platform
{
public:
    ArduinoPlat();
    virtual ~ArduinoPlat();
    virtual unsigned long getSystemUpTimeMinutes();
    virtual unsigned long getSystemUpTimeMillis();
    virtual unsigned long getSystemUpTimeMicros();
    virtual void setPinMode(int pin, PinMode mode);
    virtual void setPin(int pin, bool level);
    virtual bool getPin(int pin);
    virtual void delay(unsigned millis);
    virtual int  readAnalogPin(int pin);
    virtual long map(long x, long in_min, long in_max, long out_min, long out_max);

private:
};

#endif // X86_PLAT
#endif // ARDUINOPLAT_H

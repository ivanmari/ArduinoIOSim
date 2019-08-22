#ifndef X86_PLAT


#include "ArduinoPlat.h"
#include <Arduino.h>

ArduinoPlat::ArduinoPlat()
{
    //ctor
}

ArduinoPlat::~ArduinoPlat()
{
    //dtor
}

long
ArduinoPlat::map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return ::map(x, in_min, in_max, out_min, out_max);
}


unsigned long
ArduinoPlat::getSystemUpTimeMinutes()
{
    return millis()/60000;
}

unsigned long
ArduinoPlat::getSystemUpTimeMillis()
{
    return millis();
}

unsigned long
ArduinoPlat::getSystemUpTimeMicros()
{
    return micros();
}

void
ArduinoPlat::setPinMode(int pin, PinMode mode)
{
    switch(mode)
    {
    case PIN_INPUT_PULLUP:
        pinMode(pin, INPUT_PULLUP);
        break;

    case PIN_OUTPUT:
        pinMode(pin, OUTPUT);
        break;
    }

}

void
ArduinoPlat::setPin(int pin, bool level)
{
    if (level)
    {
        digitalWrite (pin, HIGH);
    }
    else
    {
        digitalWrite (pin, LOW);
    }
}

bool
ArduinoPlat::getPin(int pin)
{
    return digitalRead (pin);
}

int
ArduinoPlat::readAnalogPin(int pin)
{
    return analogRead(pin);
}

void
ArduinoPlat::delay(unsigned millis)
{
    return ::delay(millis);
}
#endif // X86_PLAT

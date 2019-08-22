#include "X86Platform.h"
#ifdef X86_PLAT

#include <ncurses.h>
#include <thread>
#include <chrono>
#include <iostream>


using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::steady_clock;

extern unsigned char GpioSnapshot[20];

X86Platform::X86Platform()
{

}

X86Platform::~X86Platform()
{
    //dtor
}

long
X86Platform::map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


unsigned long
X86Platform::getSystemUpTimeMinutes()
{
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count()/60000;
}

unsigned long
X86Platform::getSystemUpTimeMillis()
{
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

unsigned long
X86Platform::getSystemUpTimeMicros()
{
    return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}

void
X86Platform::setPinMode(int pin, PinMode mode)
{
    switch(mode)
    {
    case PIN_INPUT_PULLUP:
        std::cout << "Pin " << pin << " set as INPUT" << std::endl;
        GpioSnapshot[pin] &= 0xF0;
        GpioSnapshot[pin] |= 0xF0;  //Assumes PULL UP
        break;

    case PIN_OUTPUT:
        //std::cout << "Pin " << pin << " set as OUTPUT" << std::endl;G
        GpioSnapshot[pin] |= 0x0F;
        break;
    }

}

void
X86Platform::setPin(int pin, bool level)
{
    static int last_pin = pin;
    static int last_level = level;
    //std::this_thread::sleep_for(std::chrono::milliseconds(150));
    if (level)
    {
        //if(last_pin != pin || last_level != level)
        //std::cout << "pin #" << pin << "-> HIGH" << std::endl;
        //std::cout << "set pin #" << pin << " = " << std::hex << (int) GpioSnapshot[pin] <<   std::dec << std::endl;
        GpioSnapshot[pin] |= 0xF0;
        //std::cout << "set pin #" << pin << " = " << std::hex << (int) GpioSnapshot[pin] <<   std::dec << std::endl;
    }
    else
    {
        //if(last_pin != pin || last_level != level)
        //std::cout << "pin #" << pin << "-> LOW" << std::endl;
        //std::cout << "clear pin #" << pin << " = " << std::hex << (int) GpioSnapshot[pin] <<  std::dec << std::endl;
        GpioSnapshot[pin] &= 0x0F;
        //std::cout << "clear pin #" << pin << " = " << std::hex << (int) GpioSnapshot[pin] <<  std::dec << std::endl;
    }
}

bool
X86Platform::getPin(int pin)
{
    //std::this_thread::sleep_for(std::chrono::milliseconds(150));
    bool out = true;
//    static unsigned counter = 0;
    out = GpioSnapshot[pin] & 0xF0;
//    std::cout << "Reading pin #" << pin << " as " << out << std::endl;
    return out;
}

int
X86Platform::readAnalogPin(int pin)
{
    return 0;
}

void
X86Platform::delay(unsigned millis)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}
#endif // X86_PLAT

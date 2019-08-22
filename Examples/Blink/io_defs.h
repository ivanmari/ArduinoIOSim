#ifndef IO_DEFS
#define IO_DEFS

#ifdef X86_PLAT
const bool HIGH = true;
const bool LOW = false;
#else
#include <Arduino.h>
void initIO();
#endif // x86_PLAT

const int LEFT_PAD_PIN = 17;
const int RIGHT_PAD_PIN = 19;

const int HIGH_PITCH = 15;
const int LOW_PITCH = 16;

const int TREAT_RELEASE = 25;

#endif //IO_DEFS

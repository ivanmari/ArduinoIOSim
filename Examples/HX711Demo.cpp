#include "HX711/HX711Sim.h"
#include <iostream>
#include <cassert>

/**
 * Mock Platform for demonstration purposes.
 */
class DemoPlatform : public Platform {
public:
    bool pins[20];
    unsigned long micros;
    DemoPlatform() : micros(0) { for(int i=0; i<20; ++i) pins[i] = false; }
    virtual unsigned long getSystemUpTimeMicros() { return micros; }
    virtual unsigned long getSystemUpTimeMillis() { return micros / 1000; }
    virtual unsigned long getSystemUpTimeMinutes() { return micros / 60000000; }
    virtual void setPinMode(int pin, PinMode mode) {}
    virtual void setPin(int pin, bool level) { pins[pin] = level; }
    virtual bool getPin(int pin) { return pins[pin]; }
    virtual int readAnalogPin(int pin) { return 0; }
    virtual void delay(unsigned millis) { micros += millis * 1000; }
    virtual long map(long x, long in_min, long in_max, long out_min, long out_max) { return 0; }
};

int main() {
    DemoPlatform plat;
    int doutPin = 10;
    int sckPin = 11;

    HX711Sim hx711(plat, doutPin, sckPin);

    // 1. Basic Data Retrieval
    hx711.setData(0x123456);
    hx711.setReady(true);
    std::cout << "Data ready, DOUT: " << (plat.getPin(doutPin) ? "HIGH" : "LOW") << std::endl;

    int32_t val = 0;
    for (int i = 0; i < 24; ++i) {
        plat.setPin(sckPin, true);
        hx711.update();
        if (plat.getPin(doutPin)) val |= (1 << (23 - i));
        plat.setPin(sckPin, false);
        hx711.update();
    }
    std::cout << "Read value: 0x" << std::hex << val << std::dec << std::endl;

    // 25th pulse to set gain 128
    plat.setPin(sckPin, true);
    hx711.update();
    std::cout << "DOUT after 25th pulse: " << (plat.getPin(doutPin) ? "HIGH" : "LOW") << std::endl;
    plat.setPin(sckPin, false);
    hx711.update();

    // 2. Power Down
    plat.setPin(sckPin, true);
    hx711.update();
    plat.micros += 100; // Hold for > 60us
    hx711.update();
    std::cout << "Powered down: " << (hx711.isPoweredDown() ? "YES" : "NO") << std::endl;

    plat.setPin(sckPin, false);
    hx711.update();
    std::cout << "Powered down after releasing SCK: " << (hx711.isPoweredDown() ? "YES" : "NO") << std::endl;

    return 0;
}

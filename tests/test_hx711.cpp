#include <iostream>
#include <cassert>
#include <vector>
#include "HX711/HX711Sim.h"
#include "Examples/Blink/Platform.h"

class MockPlatform : public Platform {
public:
    bool pins[20];
    PinMode modes[20];
    unsigned long micros;

    MockPlatform() : micros(0) {
        for(int i=0; i<20; ++i) {
            pins[i] = false;
            modes[i] = PIN_OUTPUT;
        }
    }

    virtual unsigned long getSystemUpTimeMicros() { return micros; }
    virtual unsigned long getSystemUpTimeMillis() { return micros / 1000; }
    virtual unsigned long getSystemUpTimeMinutes() { return micros / 60000000; }
    virtual void setPinMode(int pin, PinMode mode) { modes[pin] = mode; }
    virtual void setPin(int pin, bool level) { pins[pin] = level; }
    virtual bool getPin(int pin) { return pins[pin]; }
    virtual int readAnalogPin(int pin) { return 0; }
    virtual void delay(unsigned millis) { micros += millis * 1000; }
    virtual long map(long x, long in_min, long in_max, long out_min, long out_max) { return 0; }
};

void test_hx711_basic() {
    MockPlatform plat;
    int dout = 10;
    int sck = 11;
    HX711Sim sim(plat, dout, sck);

    // Initially DOUT should be HIGH
    assert(plat.pins[dout] == true);

    // Set data and trigger ready
    int32_t testData = 0xABCDEF;
    sim.setData(testData);
    sim.setReady(true);

    // DOUT should go LOW
    assert(plat.pins[dout] == false);

    int32_t receivedData = 0;
    for (int i = 0; i < 24; ++i) {
        plat.pins[sck] = true;
        sim.update();
        if (plat.pins[dout]) receivedData |= (1 << (23 - i));
        plat.pins[sck] = false;
        sim.update();
    }
    assert(receivedData == testData);

    // 25th pulse
    plat.pins[sck] = true;
    sim.update();
    assert(plat.pins[dout] == true);
    assert(sim.getNextGain() == HX711Sim::GAIN_128);
    plat.pins[sck] = false;
    sim.update();

    std::cout << "test_hx711_basic passed!" << std::endl;
}

void test_hx711_gain_selection() {
    MockPlatform plat;
    int dout = 10;
    int sck = 11;
    HX711Sim sim(plat, dout, sck);

    // Test GAIN_32 (26 pulses)
    sim.setReady(true);
    for (int i = 0; i < 26; ++i) {
        plat.pins[sck] = true;
        sim.update();
        plat.pins[sck] = false;
        sim.update();
    }
    assert(sim.getNextGain() == HX711Sim::GAIN_32);

    // Test GAIN_64 (27 pulses)
    sim.setReady(true);
    for (int i = 0; i < 27; ++i) {
        plat.pins[sck] = true;
        sim.update();
        plat.pins[sck] = false;
        sim.update();
    }
    assert(sim.getNextGain() == HX711Sim::GAIN_64);

    std::cout << "test_hx711_gain_selection passed!" << std::endl;
}

void test_hx711_power_down() {
    MockPlatform plat;
    int dout = 10;
    int sck = 11;
    HX711Sim sim(plat, dout, sck);

    sim.setReady(true);
    assert(plat.pins[dout] == false);

    // Hold SCK HIGH for 100us
    plat.pins[sck] = true;
    sim.update();
    plat.micros += 100;
    sim.update();

    assert(sim.isPoweredDown() == true);
    assert(plat.pins[dout] == true);

    // Release SCK
    plat.pins[sck] = false;
    sim.update();
    assert(sim.isPoweredDown() == false);

    std::cout << "test_hx711_power_down passed!" << std::endl;
}

void test_hx711_not_ready_ignore_pulses() {
    MockPlatform plat;
    int dout = 10;
    int sck = 11;
    HX711Sim sim(plat, dout, sck);

    // DOUT is HIGH (not ready)
    assert(plat.pins[dout] == true);

    // Pulse SCK
    plat.pins[sck] = true;
    sim.update();
    assert(plat.pins[dout] == true); // Should stay HIGH
    plat.pins[sck] = false;
    sim.update();

    std::cout << "test_hx711_not_ready_ignore_pulses passed!" << std::endl;
}

int main() {
    test_hx711_basic();
    test_hx711_gain_selection();
    test_hx711_power_down();
    test_hx711_not_ready_ignore_pulses();
    return 0;
}

#ifndef HX711SIM_H
#define HX711SIM_H

#include "Platform.h"
#include <stdint.h>

class HX711Sim {
public:
    enum Gain {
        GAIN_128 = 1, // 25 pulses
        GAIN_32  = 2, // 26 pulses
        GAIN_64  = 3  // 27 pulses
    };

    HX711Sim(Platform& plat, int doutPin, int pdSckPin);

    // Call this in the simulation loop
    void update();

    // Set the 24-bit data to be shifted out.
    void setData(int32_t data);

    // Trigger data ready (pulls DOUT low)
    void setReady(bool ready = true);

    bool isPoweredDown() const { return _poweredDown; }
    Gain getNextGain() const { return _nextGain; }

private:
    Platform& _plat;
    int _doutPin;
    int _pdSckPin;
    int32_t _data;
    int32_t _currentShiftData;
    int _pulseCount;
    bool _lastPdSck;
    bool _ready;
    bool _poweredDown;
    unsigned long _pdSckHighStartTime;
    Gain _nextGain;
};

#endif // HX711SIM_H

#include "HX711Sim.h"

HX711Sim::HX711Sim(Platform& plat, int doutPin, int pdSckPin)
    : _plat(plat), _doutPin(doutPin), _pdSckPin(pdSckPin), _data(0),
      _currentShiftData(0), _pulseCount(0), _lastPdSck(false), _ready(false),
      _poweredDown(false), _pdSckHighStartTime(0), _nextGain(GAIN_128)
{
    _plat.setPinMode(_doutPin, PIN_OUTPUT);
    _plat.setPinMode(_pdSckPin, PIN_INPUT_PULLUP);
    _plat.setPin(_doutPin, true); // Initially high
}

void HX711Sim::setData(int32_t data) {
    _data = data & 0x00FFFFFF;
}

void HX711Sim::setReady(bool ready) {
    if (_poweredDown) return;

    _ready = ready;
    if (_ready) {
        _plat.setPin(_doutPin, false);
        _pulseCount = 0;
        _currentShiftData = _data;
    } else {
        _plat.setPin(_doutPin, true);
    }
}

void HX711Sim::update() {
    bool currentPdSck = _plat.getPin(_pdSckPin);
    unsigned long now = _plat.getSystemUpTimeMicros();

    // Power down logic
    if (currentPdSck) {
        if (!_lastPdSck) {
            _pdSckHighStartTime = now;
        } else if (!_poweredDown && (now - _pdSckHighStartTime > 60)) {
            _poweredDown = true;
            _ready = false;
            _plat.setPin(_doutPin, true);
        }
    } else {
        if (_poweredDown) {
            _poweredDown = false;
            // When PD_SCK goes low after power down, chip resets and stays in standby
            // until next conversion is ready.
        }
    }

    if (_poweredDown) {
        _lastPdSck = currentPdSck;
        return;
    }

    // Detect rising edge
    if (currentPdSck && !_lastPdSck) {
        // We only respond to pulses if data was ready OR we are in the middle of the 25-27 pulse sequence
        if (_ready || (_pulseCount >= 24 && _pulseCount < 27)) {
            _pulseCount++;

            if (_pulseCount <= 24) {
                // Shift out MSB
                bool bit = (_currentShiftData & 0x800000) != 0;
                _plat.setPin(_doutPin, bit);
                _currentShiftData <<= 1;
            } else if (_pulseCount == 25) {
                // 25th pulse pulls DOUT back to high
                _plat.setPin(_doutPin, true);
                _nextGain = GAIN_128;
                _ready = false;
            } else if (_pulseCount == 26) {
                _nextGain = GAIN_32;
            } else if (_pulseCount == 27) {
                _nextGain = GAIN_64;
            }
        }
    }

    // If we were ready and we just saw the end of the pulse train
    // we can't easily know if more pulses are coming without a timeout,
    // but typically they come in a burst.

    _lastPdSck = currentPdSck;
}

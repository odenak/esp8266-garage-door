#ifndef Led_h
#define Led_h

#include <Arduino.h>
#include <Ticker.h>

const static int LED_ON = LOW;
const static int LED_OFF = HIGH;

class Led {
    public:
        Led(byte pin);
        ~Led();
        void init();
        bool isOn();
        bool isOff();
        void on();
        void off();
        void toggle();
        static void toggle(Led* led);
        void flash(float milliseconds);
        void stopFlash();

    private:
        const byte _pin;
        int _state;
        Ticker _ticker;

        void _setState(int state);
};

#endif

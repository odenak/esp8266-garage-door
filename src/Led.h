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
        void flash(uint32_t milliseconds);
        void stopFlash();
        static bool isOn(Led* led);
        static bool isOff(Led* led);
        static void on(Led* led);
        static void off(Led* led);
        static void toggle(Led* led);
        static void flash(Led* led, uint32_t milliseconds);
        static void stopFlash(Led* led);

    private:
        const byte _pin;
        int _state;
        Ticker _ticker;

        void _setState(int state);
};

#endif

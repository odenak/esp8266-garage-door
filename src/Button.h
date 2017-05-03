#ifndef Button_h
#define Button_h

#include <Arduino.h>
#include <Ticker.h>


const static int BUTTON_UP = 0;      // switch this when wired up to non LED
const static int BUTTON_DOWN = 1;

class Button {
    public:
        Button(byte pin);
        ~Button();
        void init();
        void up();
        void down();
        void tap();
        static void up(Button* button);
        static void down(Button* button);
        static void tap(Button* button);

    private:
        const byte _pin;
        Ticker _ticker;
};

#endif

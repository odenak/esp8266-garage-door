#include "Led.h"

Led::Led(byte pin) : _pin(pin) {}

Led::~Led() {
    this->_ticker.detach();
}

void Led::init() {
    // initialize digital pin as an output.
    pinMode(this->_pin, OUTPUT);
    this->setBrightness(MAX_BRIGHTNESS);
    this->off();
}

bool Led::isOn() {
    return (this->_state == LED_ON);
}

bool Led::isOff() {
    return !this->isOn();
}

void Led::on() {
    this->_setState(LED_ON);
}

void Led::off() {
    this->_setState(LED_OFF);
}

void Led::toggle() {
    this->_setState(!this->_state);
}

void Led::blink() {
    this->toggle();
    this->_ticker.once_ms(200, &Led::toggle, this);
}

// void Led::blink(unsigned int times) {
//     // this->toggle();
//     for (int i = 0; i < times * 2, i++) {
//         this->_ticker.once_ms(interval_ms, &Led::toggle, this);
//     }
// }

void Led::flash(uint32_t milliseconds) {
    this->_ticker.attach_ms(milliseconds, &Led::toggle, this);
}

void Led::stopFlash() {
    this->_ticker.detach();
}

void Led::setBrightness(unsigned int brightness) {
    // inverse the brightness since our LED is lit when LOW
    this->_brightness = brightness < MAX_BRIGHTNESS
        ? MAX_BRIGHTNESS - brightness
        : 0;
}

bool Led::isOn(Led* led) {
    return led->isOn();
}

bool Led::isOff(Led* led) {
    return led->isOff();
}

void Led::on(Led* led) {
    led->on();
}

void Led::off(Led* led) {
    led->off();
}

void Led::toggle(Led* led) {
    led->toggle();
}

void Led::flash(Led* led, uint32_t milliseconds) {
    led->flash(milliseconds);
}

void Led::stopFlash(Led* led) {
    led->stopFlash();
}

void Led::_setState(int state) {
    this->_state = state;
    if (state == LED_ON) {
        analogWrite(this->_pin, this->_brightness);
    } else {
        digitalWrite(this->_pin, state);
    }
}

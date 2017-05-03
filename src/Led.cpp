#include "Led.h"

Led::Led(byte pin) : _pin(pin) {}

Led::~Led() {
    this->_ticker.detach();
}

void Led::init() {
    // initialize digital pin as an output.
    pinMode(this->_pin, OUTPUT);
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

void Led::toggle(Led* led) {
    led->toggle();
}

void Led::flash(float milliseconds) {
    this->_ticker.attach_ms(milliseconds, &Led::toggle, this);
}

void Led::stopFlash() {
    this->_ticker.detach();
}

void Led::_setState(int state) {
    this->_state = state;
    digitalWrite(this->_pin, state);
}

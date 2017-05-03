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

void Led::flash(uint32_t milliseconds) {
    this->_ticker.attach_ms(milliseconds, &Led::toggle, this);
}

void Led::stopFlash() {
    this->_ticker.detach();
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
    digitalWrite(this->_pin, state);
}

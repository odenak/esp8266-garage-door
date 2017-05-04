#include "Button.h"

Button::Button(byte pin) : _pin(pin) {}

Button::~Button() {
    this->_ticker.detach();
}

void Button::init() {
    this->up();
    pinMode(this->_pin, OUTPUT);
}

void Button::up() {
    digitalWrite(this->_pin, BUTTON_UP);
}

void Button::down() {
    digitalWrite(this->_pin, BUTTON_DOWN);
}

void Button::tap() {
    Serial.println("Tapping Button");
    this->down();
    this->_ticker.once_ms(200, &Button::up, this);
}

void Button::up(Button* button) {
    button->up();
}

void Button::down(Button* button) {
    button->up();
}

void Button::tap(Button* button) {
    button->tap();
}

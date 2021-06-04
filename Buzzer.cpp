//
// Created by michael on 28.05.21.
//

#include "Buzzer.h"

Buzzer::Buzzer(byte pin) {
    this->pin = pin;
    init();
}

void Buzzer::init() {
    pinMode(pin, OUTPUT);
}

void Buzzer::on() {
    digitalWrite(pin, HIGH);
}

void Buzzer::off() {
    digitalWrite(pin, LOW);
}

void Buzzer::beep(int delayBeep) {
    on();
    delay(delayBeep);
    off();
    delay(delayBeep);
}
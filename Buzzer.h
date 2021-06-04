//
// Created by michael on 28.05.21.
//

#ifndef UNTITLED_BUZZER_H
#define UNTITLED_BUZZER_H

#include <Arduino.h>

class Buzzer {
    private:
        byte pin;

    public:
        Buzzer(byte pin);

        void init();
        void beep(int delayBeep);
        void on();
        void off();
};


#endif //UNTITLED_BUZZER_H

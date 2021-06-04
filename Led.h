//
// Created by michael on 27.05.21.
//

#ifndef UNTITLED_LED_H
#define UNTITLED_LED_H

#include <Arduino.h>


class Led {
    private:
        byte pin;

    public:
        Led(byte pin);
        void init();
        void on();
        void off();
};


#endif //UNTITLED_LED_H

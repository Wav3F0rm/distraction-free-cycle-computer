#ifndef buttonDebounce_h
#define buttonDebounce_h

#include "Arduino.h"
class buttonDebounce {
    // Class code here

    // public variables can be called in the sketch
    public:
        buttonDebounce(int pin);
        int buttonFunction(int debounceDelay);

    // private variables can only be called within the class
    private:
        int _pin;
        int buttonState;
        int lastButtonState;
        unsigned long lastDebounceTime;
        unsigned long debounceDelay;

};

#endif
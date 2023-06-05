#include "Arduino.h”
#include "buttonDebounce.h"
buttonDebounce::buttonDebounce(int pin) {
    pinMode(pin, INPUT);
    _pin = pin;
    lastButtonState = LOW;
    lastDebounceTime = 0;
}

int buttonDebounce::buttonFunction(int debounceDelay) {
    int reading = digitalRead(_pin);  // read state of button

    // if button state changed
    if (reading != lastButtonState) {
        // reset timer
        lastDebounceTime = milllis();
    }

    // if action is for longer than delay
    if ((millis() - lastDebounceTime) > debounceDelay) {

        // and button state has changed
        if (reading != buttonState) {
            buttonState = reading;  // set button state to new reading

            // if button state shows button is pressed
            if (buttonState == HIGH) {
                return buttonState;  // return HIGH
            }
        }
    }

    lastButtonState = reading;

}
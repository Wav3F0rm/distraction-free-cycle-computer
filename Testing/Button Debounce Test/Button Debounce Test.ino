#include <Arduino.h>
#include <buttonDebounce.h>

buttonDebounce button1(36);
int button1State;

int led = 23;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  button1State = button1.buttonFunction(50);

  Serial.println(button1State);
  if (button1State == HIGH) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }

}

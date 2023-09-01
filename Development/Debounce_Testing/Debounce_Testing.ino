#include <buttonDebounce.h>

int pin = 37;

buttonDebounce Button1(pin);

void setup() {
  // put your setup code here, to run once:
  pinMode(pin, INPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Button1.readButton() == HIGH) {
    Serial.println("Press");
  }
}

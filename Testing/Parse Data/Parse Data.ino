#include <HardwareSerial.h>
#include <TinyGPS++.h>

HardwareSerial gpsSerial(1);  // select which UART bridge to use, UART0 is used for USB data transfer
TinyGPSPlus gps;  // creates identifier for TinyGPSPlus library

int gpsSatellites = 0;
double gpsLatitude = 0;
double gpsLongitude = 0;


void setup(){
  Serial.begin(115200); // connect serial
  gpsSerial.begin(9600, SERIAL_8N1, 22, 21); // connect gps sensor
}


void loop(){
  while(gpsSerial.available()){
    gps.encode(gpsSerial.read());
  }

  if (gps.satellites.isUpdated()) {
    numSatellites = gps.satellites.value();
    Serial.print("Satellites Connected: "); Serial.println(gpsSatellites);
  }

}
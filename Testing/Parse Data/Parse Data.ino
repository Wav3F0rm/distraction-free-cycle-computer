#include <HardwareSerial.h>
#include <TinyGPS++.h>

HardwareSerial gpsSerial(1);  // select which UART bridge to use, UART0 is used for USB data transfer
TinyGPSPlus gps;  // creates identifier for TinyGPSPlus library

int gpsSatellites = 0;

float gpsLatitude = 0;
float gpsLongitude = 0;

int gpsSpeed = 0;


void setup(){
  Serial.begin(115200); // connect serial
  gpsSerial.begin(9600, SERIAL_8N1, 22, 21); // connect gps sensor
}


void loop(){
  while(gpsSerial.available()){
    gps.encode(gpsSerial.read());
  }

  if (gps.satellites.isUpdated()) {
    gpsSatellites = gps.satellites.value();
    Serial.print("Satellites Connected: "); Serial.println(gpsSatellites);
  }

  if (gps.location.isUpdated()) {
    gpsLatitude = gps.location.lat(), 6;
    gpsLongitude = gps.location.lng(), 6;
    Serial.print("Latitude: "); Serial.println(gpsLatitude, 6);
    Serial.print("Longitude: "); Serial.println(gpsLongitude, 6);
  }

  if (gps.speed.isUpdated()) {
    gpsSpeed = gps.speed.kmph();
    Serial.print("Speed: "); Serial.print(gpsSpeed); Serial.println("km/h");
  }  

}
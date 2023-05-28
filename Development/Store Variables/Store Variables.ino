#include <HardwareSerial.h>
#include <TinyGPS++.h>

// Initialise gpsSerial using HardwareSerial.
HardwareSerial gpsSerial(1);  // Select UART1, UART0 is used for USB transfer

// Create identifier for TinyGPSPlus
TinyGPSPlus gps;


/*
Data points to store:
  - Number of Satellites | How many satellites the GPS module is connected to
  - Speed
  - Altitude
  - Heading | Direction of travel from north, represented in degrees
  - Time
  - Distance Travelled | Approximate value taken from velocity over X time interval
  - Time since start of ride/pace timer | simple 
*/
int gpsSatellites = 0;
float gpsSpeed = 0;
float gpsAltitude = 0;
float gpsHeading = 0;
float gpsTime = 0;
float distTravelled = 0;
int secondSinceStart = 0;

// helper variables to store times for speed > distance & secondSinceStart calculation
unsigned long lastMillis = 0;
unsigned long currentMillis = 0;
unsigned long millisAtStart = 0;  // needs to be set by user
bool timeStart = false;


void setup() {
  Serial.begin(115200);  // begin ESP32 Serial @ 115200 baud
  gpsSerial.begin(9600, SERIAL_8N1, 22, 21);  // begin GPS Serial @ 9600 baud (refresh rate of gps module). Use GPIO22 for RX & GPIO21 for TX

}


void loop() {
  // feed the hungry gps object using serial input from gps module
  while(gpsSerial.available()){
    gps.encode(gpsSerial.read());
  }

  if (gps.satellites.isUpdated()) {
    updateTelemetry();
  }

}


void updateTelemtry() {
  gpsSatellites = gps.satellites.value();
  gpsSpeed = gps.speed.kmph();
  gpsAltitude = gps.altitude.meters();
  gpsHeading = gps.course.deg();
  gpsTime = gps.time.value();

  /*
  speed -> distance calculation | every 5 seconds
  distance = speed/time => increment the distance by speed(kmh)/time(h)
  1/720 provides exact value for 5000ms in hours
  */
  currentMillis = millis();
  if ((currentMillis - lastMillis) > 5000) {
    lastMillis = currentMillis;
    distTravelled += (gpsSpeed * (1/720));  
  }

  // second since start
  if (timeStart == true) {
    secondSinceStart = (currentMillis-millisAtStart)/1000;
  }
}

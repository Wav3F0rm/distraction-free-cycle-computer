#include <Arduino.h>
#include <HardwareSerial.h>
#include <TinyGPS++.h>
#include <U8g2lib.h>
#include <SPI.h>

// Initialise gpsSerial using HardwareSerial.
HardwareSerial gpsSerial(1);  // Select UART1, UART0 is used for USB transfer
TinyGPSPlus gps; // Create identifier for TinyGPSPlus




/*
setup LCD functionality using U8g2 constructor
Driver: ST7565 | Display: ERC12864 | Rotation: Null (U8G2_R0)
clock: 9
data: 10
cs: 2
dc: 4
reset: 0
*/
U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI u8g2(U8G2_R0, 9, 10, 2, 4, 0);

int lcdContrast = 70;


void setup() {
  Serial.begin(115200);  // begin ESP32 Serial @ 115200 baud
  gpsSerial.begin(9600, SERIAL_8N1, 22, 21);  // begin GPS Serial @ 9600 baud (refresh rate of gps module). Use GPIO22 for RX & GPIO21 for TX

  u8g2.begin();  // initialise serial connection between arduino and display (unsure of serial port use, if gps or serial is broken then using ports 0 or 1)
  u8g2.setContrast(lcdContrast);  // sets contrast of display using lcdContrast variable
}


void loop() {
  // feed the hungry gps object using serial input from gps module
  while(gpsSerial.available()){
    gps.encode(gpsSerial.read());
  }

  /*
  everytime satellite value is updated (everytime GNSS module recieves a new set of NMEA sequences)
  updateTelemetry will be called, collecting new data from the GNSS module
  */
  if (gps.satellites.isUpdated()) {
    updateTelemetry();
  }

  u8g2.clearBuffer();
  updateLCD();  // call updateLCD, updates LCD display with new telemetry
  u8g2.sendBuffer();  
}


void updateTelemetry() {
  gpsSatellites = gps.satellites.value();
  gpsSpeed = gps.speed.kmph();
  gpsAltitude = gps.altitude.meters();
  gpsHeading = gps.course.deg();
  gpsHour = gps.time.hour() - 12;
  gpsMinute = gps.time.minute();  

  /*
  SPEED FILTERING
  if the difference in speed between current and last values is greater than 1 km/h then gpsSpeed is kept at the previous speed(Might be changed later)
  this is because rapid changes to speed could be a result of incorrect readings, so these are disregarded
  Also, sets minimum gpsSpeed to 2 km/h (Might be changed later)
  this is to prevent the inaccuracies that come with moving the gps at low speeds.
  if the reported speed is < 2 km/h gpsSpeed will be set to 0
  */
  if ((abs(gpsSpeed - gpsLastSpeed)) > 1) {
    gpsSpeed = gpsLastSpeed;
    gpsDisplaySpeed = gpsSpeed;
  } else {
    gpsDisplaySpeed = gpsSpeed;
  }
  if (gpsSpeed < 2.0) {
    gpsDisplaySpeed = 0;
  } else {
    gpsDisplaySpeed = gpsSpeed;
  }

  /*
  speed -> distance calculation | every 3 seconds
  distance = speed/time => increment the distance by speed(kmh)/time(h)
  1/1200 provides exact value for 3000ms in hours
  */
  currentMillis = millis();
  if ((currentMillis - lastMillis) > 3000) {
    lastMillis = currentMillis;
    distTravelled += (gpsSpeed * (1/1200));  
  }

  // second since start
  if (timeStart == true) {
    secondSinceStart = (currentMillis-millisAtStart)/1000;
  }

  gpsLastSpeed = gpsSpeed;
}


void updateLCD() {
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);

  u8g2.drawStr(0, 0, ("Satellites: " + String(gpsSatellites)).c_str());
  u8g2.drawStr(0, 7, ("Time: " + String(gpsHour) + ":" + String(gpsMinute)).c_str());
  u8g2.drawStr(0, 14, ("Speed: " + String(gpsDisplaySpeed)).c_str());
  u8g2.drawStr(0, 21, ("Altitude: " + String(gpsAltitude)).c_str());
  u8g2.drawStr(0, 28, ("Heading: " + String(gpsHeading)).c_str());
}

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

#include <TinyGPS++.h>
#include <HardwareSerial.h>


// Initialise LCD Module, using u8g2 library, and the SPI communication protocol
U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI u8g2(U8G2_R3, /*Clock:*/ 9,/*Data:*/ 10, /*CS:*/ 2, /*DC:*/ 4, /*Reset:*/ 0);

int lcdContrast = 70;

// Define Vertical Layout for Display
#define Vertical2_width 64
#define Vertical2_height 128
static unsigned char Vertical2_bits[] = {
   0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
   0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
   0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
   0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
   0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
   0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
   0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
   0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
   0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
   0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
   0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x00, 0xfc, 0x3f, 0x00, 0xff, 0x0f,
   0x0c, 0x00, 0x03, 0x03, 0xc0, 0xc0, 0x00, 0x30, 0x02, 0x00, 0x84, 0x00, 0x00, 0x21, 0x00, 0x40, 0x02, 0x00, 0x84, 0x00, 0x00, 0x21, 0x00, 0x40,
   0x01, 0x00, 0x48, 0x00, 0x00, 0x12, 0x00, 0x80, 0x01, 0x00, 0x48, 0x00, 0x00, 0x12, 0x00, 0x80, 0x01, 0x00, 0x48, 0x00, 0x00, 0x12, 0x00, 0x80,
   0x01, 0x00, 0x48, 0x00, 0x00, 0x12, 0x00, 0x80, 0x02, 0x00, 0x84, 0x00, 0x00, 0x21, 0x00, 0x40, 0x02, 0x00, 0x84, 0x00, 0x00, 0x21, 0x00, 0x40,
   0x0c, 0x00, 0x03, 0x03, 0xc0, 0xc0, 0x00, 0x30, 0xf0, 0xff, 0x00, 0xfc, 0x3f, 0x00, 0xff, 0x0f };

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
int gpsHour = 0;
int gpsMinute = 0;
char gpsTime[50];

float distTravelled = 0;

unsigned int i = 0;
float totalSpeed = 0;
float averageSpeed = 0;

float gpsLastSpeed = 0;
float gpsDisplaySpeed = 0;

// helper variables to store times for speed > distance & secondSinceStart calculation
unsigned long lastMillis = 0;
unsigned long currentMillis = 0;

unsigned long durationSeconds = 0;
unsigned long durationMinutes = 0;
unsigned long durationHours = 0;
char durationFormatted[50];


// Initialise gpsSerial using HardwareSerial.
HardwareSerial gpsSerial(1);  // Select UART1, UART0 is used for USB transfer
TinyGPSPlus gps; // Create identifier for TinyGPSPlus


// This code will run once at the beginning of the project
void setup(void) {
  Serial.begin(115200);  // begin ESP32 Serial @ 115200 baud
  gpsSerial.begin(9600, SERIAL_8N1, 22, 21);  // begin GPS Serial @ 9600 baud (refresh rate of gps module). Use GPIO22 for RX & GPIO21 for TX

  u8g2.begin();
  u8g2.setContrast(lcdContrast);
  
}

void loop(void) {
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

  // draw loop
  u8g2.clearBuffer();
  updateLCD();
  u8g2.sendBuffer();    

}

// 
void updateLCD() {
  u8g2.drawXBM(0, 0, Vertical2_width, Vertical2_height, Vertical2_bits);  // x-position, y-position, w-dimension, h-dimension, bitmap
  
  u8g2.setFontPosBottom();
  u8g2.setFontDirection(0);

  u8g2.setFont(u8g2_font_luBS18_tf);
  u8g2.drawStr(2, 38, String(gpsDisplaySpeed, 1).c_str());

  u8g2.setFont(u8g2_font_t0_12_tf);
  u8g2.drawStr(38, 46, "km/h");
  
  u8g2.drawStr(5, 69, gpsTime);  // Draw time in first box
  u8g2.drawStr(5, 84, (String(distTravelled, 2) + " km").c_str());  // Draw distance travelled in second box
  u8g2.drawStr(5, 99, (String(averageSpeed, 1) + " km/h").c_str());  // Draw average speed in third box
  u8g2.drawStr(5, 114, durationFormatted);  // Draw duration of ride speed in third box
}

void updateTelemetry() {
  /*
  update telemetry gained from gps
  No. of satellites
  Speed - filtered
  Time
  Distance travelled - calculated from speed
  Average speed - calculated from distance
  Length of ride
  */  

  gpsSatellites = gps.satellites.value();
  gpsSpeed = gps.speed.kmph();

  gpsHour = gps.time.hour();
  if (gpsHour < 12){
    gpsHour += 12;
  } else if (gpsHour < 23){
    gpsHour -= 12;
  }
  gpsMinute = gps.time.minute();
  sprintf(gpsTime, "%02d:%02d", gpsHour, gpsMinute);  

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
  if the time between the last cycle and the current cycle is > 3000ms / 3s
  set last cycle time to current cycle time
  gpsDisplaySpeed(km/h) -> m/s * time in seconds = meters travelled since last cycle -> km travelled since last cycle
  */
  currentMillis = millis();
  if((millis() - lastMillis) > 3000){
    i ++;
    lastMillis = currentMillis;
    distTravelled += ((gpsDisplaySpeed/3.6)*3)/1000;

    totalSpeed += gpsDisplaySpeed;
    averageSpeed = totalSpeed/i;
  }

  // Length of ride
  durationSeconds = currentMillis / 1000;
  durationMinutes = durationSeconds / 60;
  durationHours = durationMinutes / 60;
  currentMillis %= 1000;
  durationSeconds %= 60;
  durationMinutes %= 60;
  durationHours %= 24;
  sprintf(durationFormatted, "%02d:%02d:%02d", durationHours, durationMinutes, durationSeconds);

  gpsLastSpeed = gpsSpeed;
}
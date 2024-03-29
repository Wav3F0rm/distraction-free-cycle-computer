#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

/*
clock: 9
data: 10
cs: 2
dc: 4
reset: 0
*/
U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI u8g2(U8G2_R0, 9, 10, 2, 4, 0);

#define Goose_width 47
#define Goose_height 64
static unsigned char Goose_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00,
   0xe0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x7f, 0x00, 0x00, 0x00, 0x00,
   0xf0, 0xff, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x00, 0x00, 0x00, 0x00,
   0xf0, 0xff, 0x01, 0x00, 0x00, 0x00, 0xf8, 0xf9, 0x01, 0x00, 0x00, 0x00,
   0x78, 0xe0, 0x01, 0x00, 0x00, 0x00, 0x18, 0xe0, 0x03, 0x00, 0x00, 0x00,
   0x04, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x02, 0xc0, 0x03, 0x00, 0x00, 0x00,
   0x02, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0x00,
   0x00, 0xe0, 0x01, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x01, 0x00, 0x00, 0x00,
   0x00, 0xf0, 0x01, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0x00,
   0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00,
   0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00,
   0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x03, 0x00, 0x00, 0x00, 0x00,
   0xf0, 0x03, 0x0f, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x7f, 0x00, 0x00, 0x00,
   0xf8, 0xff, 0xff, 0x03, 0x00, 0x00, 0xf8, 0xff, 0xff, 0x07, 0x00, 0x00,
   0xf8, 0xff, 0xff, 0x1f, 0x00, 0x00, 0xf8, 0xff, 0xff, 0x3f, 0x00, 0x00,
   0xf8, 0xff, 0xff, 0xff, 0x00, 0x11, 0xf8, 0xff, 0xff, 0xff, 0x83, 0x19,
   0xf0, 0xff, 0xff, 0xff, 0xcf, 0x0d, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x0f,
   0xf0, 0xff, 0xff, 0xff, 0xff, 0x37, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x3f,
   0xe0, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x03,
   0xc0, 0xff, 0xff, 0xff, 0xff, 0x07, 0x80, 0xff, 0xff, 0xff, 0xff, 0x07,
   0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0xfe, 0xff, 0xff, 0x7f, 0x00,
   0x00, 0xfc, 0xff, 0xff, 0x0f, 0x00, 0x00, 0xe0, 0xdf, 0x1f, 0x00, 0x00,
   0x00, 0x00, 0x0c, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1e, 0x00, 0x00,
   0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x18, 0x00, 0x00,
   0x00, 0x00, 0x04, 0x30, 0x00, 0x00, 0x00, 0x00, 0x04, 0x30, 0x00, 0x00,
   0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
   0x00, 0xf0, 0x03, 0x10, 0x00, 0x00, 0x00, 0xf0, 0x01, 0x30, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void setup(void) {

  u8g2.begin();
  u8g2.setContrast(70);
  
}

void drawGoose() {
  u8g2.drawXBM(41, 0, Goose_width, Goose_height, Goose_bits);  // x-position, y-position, w-dimension, h-dimension, bitmap
}

void loop(void) {

  // draw loop
  u8g2.clearBuffer();
  drawGoose();
  u8g2.sendBuffer();    

}
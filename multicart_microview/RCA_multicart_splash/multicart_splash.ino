// RCA Multicart - Rotary Switch Alternative
// Writes Multicart logo splash to EEPROM 
// written by FliP 2018
// 
// v1.0 - initial release
//
//////////////////////////////////////////////////////
#include <EEPROM.h>
#include <MicroView.h>

int addr =0;
const unsigned char splash[] = {
0x00, 0x00, 0x00, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x06, 0x06, 0xFE, 0xFE, 0xFE, 0xFE,
0xFE, 0xFE, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFC, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x06,
0x06, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFC, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF8,
0xFE, 0xFE, 0xFE, 0xFE, 0x06, 0x06, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x07, 0x1F, 0xFF, 0xFF,
0xFF, 0xFF, 0xFD, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0,
0xC0, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x7E, 0x1E, 0x00, 0x80, 0xE0, 0xFC, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0x1F, 0x07, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
0x00, 0xE0, 0xF0, 0x18, 0x08, 0x78, 0x18, 0x00, 0x80, 0x80, 0xE0, 0xF0, 0x88, 0x00, 0x00, 0x80,
0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xF8, 0x3C,
0x06, 0x06, 0x86, 0x98, 0x18, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x08, 0xF8, 0xF8, 0x08, 0x08, 0x00, 0x00, 0x00, 0x08, 0xF8, 0xF8, 0x08, 0x08,
0x80, 0x81, 0x43, 0x7F, 0x3C, 0x00, 0x00, 0x00, 0x78, 0x7F, 0x67, 0x20, 0x30, 0x00, 0x71, 0x7C,
0x27, 0x21, 0x70, 0x7E, 0x2F, 0x21, 0x00, 0x78, 0x7C, 0x63, 0x21, 0x30, 0x7C, 0x3F, 0x33, 0x18,
0x72, 0x7D, 0x3F, 0x13, 0x00, 0x3C, 0x7E, 0x41, 0x40, 0x20, 0x1F, 0x0F, 0x00, 0x00, 0x00, 0x00,
0x40, 0x40, 0x70, 0x7F, 0x4F, 0x00, 0x00, 0x00, 0x40, 0x40, 0x70, 0x7F, 0x4F, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x06, 0x38, 0xC0,
0x00, 0xC0, 0x38, 0x06, 0xFE, 0x00, 0xF0, 0x00, 0x00, 0x00, 0xF0, 0x00, 0xFE, 0x10, 0xFE, 0x10,
0xF2, 0x00, 0xE0, 0x10, 0x10, 0x10, 0x20, 0x00, 0x20, 0x90, 0x90, 0x90, 0xE0, 0x00, 0xF0, 0x20,
0x10, 0x10, 0xFE, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x03,
0x04, 0x03, 0x00, 0x00, 0x07, 0x00, 0x03, 0x04, 0x04, 0x02, 0x07, 0x00, 0x07, 0x00, 0x07, 0x04,
0x07, 0x00, 0x03, 0x04, 0x04, 0x04, 0x03, 0x00, 0x03, 0x04, 0x04, 0x04, 0x07, 0x00, 0x07, 0x00,
0x00, 0x00, 0x07, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void setup() {
  Serial.begin(9600);
  EEPROM.write(0,0); //start slot
}

void loop() {
// put the splash screen in EEPROM from 0x0010 onwards to save on RAM
  unsigned int i,j;
    for (i=0; i<384; i++) {
    EEPROM.write(i+16,splash[i]);
    Serial.println(i);
    }
while(1) {}
}

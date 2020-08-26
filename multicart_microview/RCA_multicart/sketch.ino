
// RCA Multicart - Rotary Switch Alternative
// Using a Sparkview Microview and a rotary switch, cartridge slots can be selected
// written by FliP 2018
// 
// v1.0 - initial release
// v1.1 - uses logo stored in EEPROM to reduce variable usage and critical memory usage
//
//####################################################################################

#include <MicroView.h>
#include <Bounce2.h>
#include <EEPROM.h>

static const int kPin_A12  = A0; // Microview pin 7 / A0
static const int kPin_A13  = A1; // Microview pin 6 / A1
static const int kPin_A14  = A2; // Microview pin 5 / A2
static const int kPin_A15  = A3; // Microview pin 4 / A3

static const int kPin_A16  = A4; // Microview pin 3 / A4
static const int kPin_A17  = A5; // Microview pin 2 / A5
static const int kPin_A18  = 6; // Microview pin 14 / D6

static const int kPin_push = 5; // Microview 13 / D5
const unsigned int usedSlots[] = {0,1,2,3,4,5,6,7,8,9,10,11,
                                  16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
                                  32,33,34,35,36,37,38,39,
                                  48,49,50,51,52,
                                  64,65,66,67,68,69,70,
                                  80,81,82};
const char *banks[] = {"RCA Orig.","PAL Clones","P.Robson","L.Romanow","Other","CHIP-8"};
const char *slots[] = {"Studio II ROM","Space War","Fun with  Numbers","Tennis/   Squash","Baseball","Speedway/ Tag","GunfighterMoonship  Battle","School    House I","School    House II","Blackjack","Bingo","Biorytm",
                       "Grand Pack","Space War","Fun with  Numbers","Tennis/   Squash","Baseball","Speedway/ Tag","GunfighterMoonship  Battle","School    House I","School    House II","Blackjack","Bingo","Biorytm","Concentr. Match","Star Wars","Pinball",
                       "Asteroids","Berzerk","Combat","Hockey/   Pong","Invaders","Kaboom","Pacman","Scramble",
                       "Climber","Outbreak","Rocket","TVA2012","Fifteen",
                       "Prog Cart","H/W Test","ROM       Checksum","Mem Test","Keypad    Test","RCA Test","RCA Demo",
                       "Addition","Tetris","Airplane"
                      };

int slotCount = 50;

static int pinA = 2; // Our first hardware interrupt pin is digital pin 2
static int pinB = 3; // Our second hardware interrupt pin is digital pin 3
volatile byte aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile byte encoderPos = 128; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
volatile byte oldEncPos = 128; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent

Bounce debounce_push = Bounce();
long position = 0;
unsigned int selectedSlot = EEPROM.read(0);

void setup() {
  Serial.begin(9600);
  uView.begin();
  uView.clear(PAGE);
  pinMode(kPin_A12, OUTPUT);
  digitalWrite(kPin_A12,LOW);
  pinMode(kPin_A13, OUTPUT);
  digitalWrite(kPin_A13,LOW);
  pinMode(kPin_A14, OUTPUT);
  digitalWrite(kPin_A14,LOW);
  pinMode(kPin_A15, OUTPUT);
  digitalWrite(kPin_A15,LOW);
  pinMode(kPin_A16, OUTPUT);
  digitalWrite(kPin_A16,LOW);
  pinMode(kPin_A17, OUTPUT);
  digitalWrite(kPin_A17,LOW);
  pinMode(kPin_A18, OUTPUT);
  digitalWrite(kPin_A18,LOW);
  SetSlot(usedSlots[selectedSlot]);

  
  pinMode(kPin_push, INPUT_PULLUP);
  debounce_push.attach(kPin_push);
  debounce_push.interval(100);
  pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  attachInterrupt(0,PinA,RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1,PinB,RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)

  //splash
  // width: 64 height: 48 = 384 bytes
  // stored in EEPROM from position 16 to 400

unsigned int i,j;
for (i=0; i<6; i++) {
    uView.setPageAddress(i);
    uView.setColumnAddress(0);
    for (j=0;j<0x40;j++) {
      uView.data(EEPROM.read(i*0x40+j+16));
    }
}
delay(4000);
oldEncPos = encoderPos;
}

void PinA(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
  if(reading == B00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos --; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinB(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos ++; //increment the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

// set IO pins on microView to selected address/slot
void SetSlot(int slot) {  
  digitalWrite(kPin_A12, (slot & 1)?HIGH:LOW);
  digitalWrite(kPin_A13, (slot & 2)?HIGH:LOW);
  digitalWrite(kPin_A14, (slot & 4)?HIGH:LOW);
  digitalWrite(kPin_A15, (slot & 8)?HIGH:LOW);
  digitalWrite(kPin_A16, (slot & 16)?HIGH:LOW);
  digitalWrite(kPin_A17, (slot & 32)?HIGH:LOW);
  digitalWrite(kPin_A18, (slot & 64)?HIGH:LOW);
}

void loop() {
    if(oldEncPos != encoderPos) {
        if (oldEncPos < encoderPos) { // CCW
          if (selectedSlot > 0) {selectedSlot--;} else {selectedSlot = slotCount - 1;}
          } else { //CW
          if (selectedSlot < (slotCount-1)) {selectedSlot++;} else {selectedSlot = 0;}
        }
      oldEncPos = encoderPos;
    }    

      // write the title to the screen
      uView.clear(PAGE);
      uView.setFontType(0);
      uView.setCursor(0,0);
      uView.print(banks[usedSlots[selectedSlot] >> 4]);
      uView.line(0,8,63,8);
      uView.setCursor(0,12);
      uView.print(slots[selectedSlot]);
      uView.display();

      // check for button press
      debounce_push.update();
      int btnPushed = debounce_push.fell();
      // if button was pressed, flash screen, set the new selected slot and store selected slot in EEPROM (retrieve after power down)
      if (btnPushed) {
        SetSlot(usedSlots[selectedSlot]);
        uView.invert(1);
        delay(250);
        uView.invert(0);
        EEPROM.write(0,selectedSlot);
      }
}

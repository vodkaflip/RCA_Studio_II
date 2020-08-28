# Alternative for Hex Selection

Revisions 4.5 and 5.0 of the RCA Studio II Multicart can accomodate a Sparkfun Microview module with built-in OLED screen for selecting the ROM slots. Selection is done using a single rotary encoder: dial to select a slot and push to activate. The console then needs to be reset to start the game.

## Hardware

The two hex encoders and the resistor network can be removed. The following components need to be added:

Mechanical Encoder: Mouser PEC12R-4120F-S0012 (https://www.mouser.be/ProductDetail/652-PEC12R-4120F-S12)
Sparkfun Microview: https://www.mouser.be/Search/Refine?Keyword=microview
C1-C4: 0.1uF
optional: a 6-pin header to connect an FTDI interface that allows the Microview module to be reprogrammed (if it is not socketed)


## Software
### Splash Screen

### Main

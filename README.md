# A Meris Preset Switch Extension - MidiController
An arduino based midi controller extension for the Meris Preset Switch.
It is optimized for the Meris Polymoon, but can be used for all Meris pedals.
ATTENTION: I used an arduino clone from robotdyn - mini pro 3.3v! www.robotdyn.com 

With the activation of a switch, you can change the MIDI Adress, to send your instructions to another device


MIDI baudrate: 31250


                                                                    A7   A6   A5   A4       -- SCL  SDA    -- I2C (TWI) support
    Control Change Table for Meris Polymoon:                        0    0    0    0   
    CC# 04 - Exp. pedal    CC# 22 - early modulation       SS --  0 10            09 0
    CC# 09 - dottet 8th    CC# 23 - feedback filter      MOSI --  0 11            08 0
    CC# 14 - bypass        CC# 24 - delay level          MISO --  0 12            07 0
    CC# 15 - tempo         CC# 25 - late modulation       SCK --  0 13            06 0
    CC# 16 - time          CC# 26 - dyn flanger mode              0 A0            05 0  
    CC# 17 - feedback      CC# 27 - dyn flanger speed             0 A1            04 0
    CC# 18 - mix           CC# 28 - tap                           0 A2            03 0
    CC# 19 - multiply      CC# 29 - phaser mode                   0 A3            02 0
    CC# 20 - dimension     CC# 30 - flanger feedback              0 VCC>-------> GND 0 >---- VCC -> For regulated 3.3v supply
    CC# 21 - dynamics      CC# 31 - half speed                    0 RST          RST 0 
                                                                  0 GND          RX1 0      
    CC# 00 - bank select                                          0 RAW>-------> TX0 0 >---- RAW -> For supplying a raw voltage to board (max 16VDC)
                                                                    0  0  0  0  0  0
                                                                GRN TX0 RX1 VCC GND BLK
   
  
     
     
**HOW DOES CONTROL CHANGE WORK?**
* 0xnc, 0xcc, 0xvv
** n is the status (0xB)
** c is the MIDI channel (e.g. 0)
** cc is the controller number (0-127)
** vv is the controller value (0-127)


**HOW DOES PROGRAM CHANGE WORK?**
* 0xnc, 0xpp
** n is the status (0xC)
** c is the channel (e.g. 0)
** pp is the patch number (0-127)

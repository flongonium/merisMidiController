/*                                                                  Arduino Mini Pro Clone
 *  MIDI Serial speed: 31250 - For debugging use 38400                        
 *                                                                  A7   A6   A5   A4       -- SCL  SDA    -- I2C (TWI) support
 *  Control Change Table for Meris Polymoon:                        0    0    0    0   
 *  CC# 04 - Exp. pedal    CC# 22 - early modulation       SS --  0 10            09 0
 *  CC# 09 - dottet 8th    CC# 23 - feedback filter      MOSI --  0 11            08 0
 *  CC# 14 - bypass        CC# 24 - delay level          MISO --  0 12            07 0
 *  CC# 15 - tempo         CC# 25 - late modulation       SCK --  0 13            06 0
 *  CC# 16 - time          CC# 26 - dyn flanger mode              0 A0            05 0  
 *  CC# 17 - feedback      CC# 27 - dyn flanger speed             0 A1            04 0
 *  CC# 18 - mix           CC# 28 - tap                           0 A2            03 0
 *  CC# 19 - multiply      CC# 29 - phaser mode                   0 A3            02 0
 *  CC# 20 - dimension     CC# 30 - flanger feedback              0 VCC          GND 0      -- VCC -> For regulated 3.3v supply
 *  CC# 21 - dynamics      CC# 31 - half speed                    0 RST          RST 0 
 *                                                                0 GND          RX1 0      
 *  CC# 00 - bank select                                          0 RAW          TX0 0      -- RAW -> For supplying a raw voltage to board (max 16VDC)
 *                                                                  0  0  0  0  0  0
 *                                                              GRN TX0 RX1 VCC GND BLK
 *  Note on  = 0x90 
 *  Note off = 0x80
 *  127, HEX 7F, 1111111
 *  
 *  
 *  ********HOW DOES CONTROL CHANGE WORK?*******
 *  0xnc, 0xcc, 0xvv
      n is the status (0xB)  
      c is the MIDI channel (e.g. 0)
      cc is the controller number (0-127)
      vv is the controller value (0-127)


 *  ********HOW DOES PROGRAM CHANGE WORK?*******
 *  0xnc, 0xpp
      n is the status (0xC)
      c is the channel (e.g. 0)
      pp is the patch number (0-127)
 */

/*-------------------------------------- Variables declaration --------------------------------------*/

#include "midiCommands.h"

struct inputSwitch
{
	int switchPin;
	int ledPin;
	int switchState;
	int switchStateOld;
	int midiMessageType;
};

struct inputSelector
{
	int switchPin;
	int ledPin;
	int switchState;
	int switchStateOld;
};

struct inputSwitch inputs[] =
{
	 {2, 8, 0, 0, 1}    // Preset - Switch 1, LED 1 -> permanent on
	,{3, 8, 0, 0, 2}   // Preset - Switch 2, LED 2 -> permanent on
	,{4, 8, 0, 0, 3}   // Preset - Switch 3, LED 3 -> permanent on
	,{5, 8, 0, 0, 4}   // Preset - Switch 4, LED 4 -> permanent on
	,{6, 8, 0, 0, 5}   // Change Up  - Switch 5, LED 5 -> momentary on
	,{7, 8, 0, 0, 6}   // Change Dwn - Switch 6, LED 6 -> momentary on
	,{0, 0, 0, 0, 0}    // End of list marker
};

struct inputSelector selector_1[] =
{
	 {A4, 8, 0, 0}   // Select 1 - LED 7 -> permanent on
	,{A5, 8, 0, 0}   // Select 2 - LED 8 -> permanent on
	,{0, 0, 0, 0}    // End of list marker
};


/*-------------------------------------- SETUP --------------------------------------*/

void setup()
{
  Serial.begin(31250); //MIDI rate = 31250

  //set up switches and leds
  pinsSetup();
  delay(50);

  //Set the right channel
  //channelSelector();
  delay(50);

  //select first bank on channel one
  midiSend_CC(midiChannelCC, 0, 0);
  delay(50);
}




/*-------------------------------------- MAIN PROGRAM--------------------------------------*/
void loop()
{
  switchEvents();
  //channelSelector();
}

/*-------------------------------------- SETUPS --------------------------------------*/

void pinsSetup()
{
	for(int i=0; inputs[i].switchPin != 0; ++i)
	{
		pinMode(inputs[i].switchPin, INPUT_PULLUP);
		pinMode(inputs[i].ledPin, OUTPUT);
	}
  for(int i=0; selector_1[i].switchPin != 0; ++i)
  {
    pinMode(selector_1[i].switchPin, INPUT_PULLUP);
  }
  delay(50);
}

/*-------------------------------------- SWICHT EVENTS --------------------------------------*/

void switchEvents()
{
	for(int i=0; inputs[i].switchPin != 0; ++i)
	{
		inputs[i].switchState = digitalRead(inputs[i].switchPin);

		if(inputs[i].switchState != inputs[i].switchStateOld)
		{
			if(inputs[i].switchState == HIGH)
			{
				switch (inputs[i].midiMessageType)
				{
					case 1:
						midiSend_PC(midiChannelPC, 1);
						delay(50);
						break;
					case 2:
						midiSend_PC(midiChannelPC, 2);
						delay(50);
						break;
					case 3:
						midiSend_PC(midiChannelPC, 3);
						delay(50);
						break;
					case 4:
						midiSend_PC(midiChannelPC, 4);
						delay(50);
						break;
					case 5:
						//midiSend_bankUp(midiChannelCC);
           ++midiChannelPC;
						delay(50);
						break;
					case 6:
						//midiSend_bankDown(midiChannelCC);
           --midiChannelPC;
						delay(50);
						break;
					default:
						break;
				}
			}
		}
		inputs[i].switchStateOld = inputs[i].switchState;	
	}
}

void channelSelector()
{
	for (int i=0; selector_1[i].switchPin != 0; i++)
	{
		selector_1[i].switchState = digitalRead(selector_1[i].switchPin);

		if (selector_1[i].switchState != selector_1[i].switchStateOld)
		{
			if (selector_1[i].switchPin == A4 && selector_1[i].switchState == HIGH)
			{
				midiChannelCC = 0xB1;
			}
			else if (selector_1[i].switchPin == A5 && selector_1[i].switchState == HIGH)
			{
				midiChannelCC = 0xB2;
			}
		}
		selector_1[i].switchStateOld = selector_1[i].switchState;
	}
}

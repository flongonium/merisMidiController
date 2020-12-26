/*
Written by Luca Becci
26.12.2020
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
				midiChannelCC = 0xB0;
			}
			else if (selector_1[i].switchPin == A5 && selector_1[i].switchState == HIGH)
			{
				midiChannelCC = 0xB1;
			}
		}
		selector_1[i].switchStateOld = selector_1[i].switchState;
	}
}

#include "Arduino.h"
//The setup function is called once at startup of the sketch
/*
Written by Luca Becci
26.12.2020
*/

/*-------------------------------------- Variables declaration --------------------------------------*/

#include "midiCommands.h"

int layerVal = 0;

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
	 {2, 6, 0, 0, 1}    // Preset - Switch 1, LED 1 -> permanent on
	,{3, 7, 0, 0, 2}   // Preset - Switch 2, LED 2 -> permanent on
	,{4, 8, 0, 0, 3}   // Preset - Switch 3, LED 3 -> permanent on
	,{5, 9, 0, 0, 4}   // Preset - Switch 4, LED 4 -> permanent on
	,{0, 0, 0, 0, 0}    // End of list marker
};

struct inputSelector selector_1[] =
{
	 {A4, 10, 0, 0}   // Select 1 - LED 5 -> permanent on
	,{A5, 11, 0, 0}   // Select 2 - LED 6 -> permanent on
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
  toggleChannel();
  //channelSelector();
  layerVal = setLayer(layerVal);
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

/*-------------------------------------- SWICHT EVENTS --------------------------------------*/

void toggleChannel()
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
						midiSend_PC(midiChannelPC, 1 + layerVal);
						delay(50);
						break;
					case 2:
						midiSend_PC(midiChannelPC, 2 + layerVal);
						delay(50);
						break;
					case 3:
						midiSend_PC(midiChannelPC, 3 + layerVal);
						delay(50);
						break;
					case 4:
						midiSend_PC(midiChannelPC, 4 + layerVal);
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

int setLayer(int layerVal)
{
	int val = digitalRead(inputs[0].switchPin);
	if (val == HIGH)
	{
		int val_up = digitalRead(inputs[2].switchPin);
		int val_dwn = digitalRead(inputs[3].switchPin);
		if (val_up == val)
		{
			layerVal = layerVal + 4;
			if (layerVal > 12)
			{
				layerVal = 0;
			}
		}
		else if (val_dwn == val)
		{
			layerVal = layerVal - 4;
			if (layerVal < 0)
			{
				layerVal = 0;
			}
		}
		else
		{
			break;
		}
	}
	return layerVal;
}


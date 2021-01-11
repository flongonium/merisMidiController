#include "Arduino.h"
//The setup function is called once at startup of the sketch
/*
Written by Luca Becci
26.12.2020
*/

/*-------------------------------------- Variables declaration --------------------------------------*/

#include "SoftwareSerial.h"

SoftwareSerial merisSerial(30, 12); // rx = 30 --> stays like written on board, tx is new on pin 12

int patchNumber  = 1;
int bank         = 1;

byte midiChannelCC = 0xB0;     // 176, HEX B0 -> this is channel 0, must match with midi device
byte midiChannelPC = 0xC0;     // 192, HEX C0 -> this is preset 0, let's start always with this preset

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
	 {13, 6, 0, 0, 1}    // Preset - Switch 1, LED 1 -> permanent on
	,{A0, 7, 0, 0, 2}   // Preset - Switch 2, LED 2 -> permanent on
	,{A1, 8, 0, 0, 3}   // Preset - Switch 3, LED 3 -> permanent on
	,{A2, 9, 0, 0, 4}   // Preset - Switch 4, LED 4 -> permanent on
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
  Serial.begin(9600); // for debugging
  merisSerial.begin(31250);

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
  //layerVal = setLayer(layerVal);
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


/*-------------------------------------- MIDI MAIN MESSAGES --------------------------------------*/

void midiSend_CC(byte midiChannel, byte controllerNumber , byte controllerVal) // midiChannel = number of MIDI channel, controllerNumber = CC Message, controllerVal = data Value between 0-127
{
	merisSerial.write(midiChannel);
	merisSerial.write(controllerNumber);
	merisSerial.write(controllerVal);
}

void midiSend_PC(byte midiChannel, byte patchNumber) // midiChannel = number of MIDI channel, patchNumber = preset number between 0-127 (Meris can handle only 16)
{
	merisSerial.write(midiChannel);
	merisSerial.write(patchNumber);
}

void midiSend_bankUp(byte midiChannel)
{
	++bank;
	merisSerial.write(midiChannel);
	merisSerial.write(byte(0));
	merisSerial.write(byte(bank));
}

void midiSend_bankDown(byte midiChannel)
{
	--bank;
	merisSerial.write(midiChannel);
	merisSerial.write(byte(0));
	merisSerial.write(byte(bank));
}

void midiSend_presetUp(byte midiChannel)
{
	++patchNumber;
	merisSerial.write(midiChannel);
	merisSerial.write(byte(patchNumber));
}

void midiSend_presetDown(byte midiChannel)
{
	--patchNumber;
	merisSerial.write(midiChannel);
	merisSerial.write(byte(patchNumber));
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
	}
	return layerVal;
}


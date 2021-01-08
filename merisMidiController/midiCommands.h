/*
 * midiCommands.h
 *
 *  Created on: 08.01.2021
 *      Author: lucab
 */

#ifndef MIDICOMMANDS_H_
#define MIDICOMMANDS_H_

int patchNumber  = 1;
int bank         = 1;

byte midiChannelCC = 0xB0;     // 176, HEX B0 -> this is channel 0, must match with midi device
byte midiChannelPC = 0xC0;     // 192, HEX C0 -> this is preset 0, let's start always with this preset

/*-------------------------------------- MIDI MAIN MESSAGES --------------------------------------*/

void midiSend_CC(byte midiChannel, byte controllerNumber , byte controllerVal) // midiChannel = number of MIDI channel, controllerNumber = CC Message, controllerVal = data Value between 0-127
{
	Serial.write(midiChannel);
	Serial.write(controllerNumber);
	Serial.write(controllerVal);
}

void midiSend_PC(byte midiChannel, byte patchNumber) // midiChannel = number of MIDI channel, patchNumber = preset number between 0-127 (Meris can handle only 16)
{
	Serial.write(midiChannel);
	Serial.write(patchNumber);
}

void midiSend_bankUp(byte midiChannel)
{
	++bank;
	Serial.write(midiChannel);
	Serial.write(byte(0));
	Serial.write(byte(bank));
}

void midiSend_bankDown(byte midiChannel)
{
	--bank;
	Serial.write(midiChannel);
	Serial.write(byte(0));
	Serial.write(byte(bank));
}

void midiSend_presetUp(byte midiChannel)
{
	++patchNumber;
	Serial.write(midiChannel);
	Serial.write(byte(patchNumber));
}

void midiSend_presetDown(byte midiChannel)
{
	--patchNumber;
	Serial.write(midiChannel);
	Serial.write(byte(patchNumber));
}

//void switchChannel(bool up_or_down)
//{
//  if(up_or_down == 0)
//  {
//    ++midiChannelCC;
//  }
//  else if(up_or_down == 1)
//  {
//    --midiChannelCC;
//  }
//}




#endif /* MIDICOMMANDS_H_ */

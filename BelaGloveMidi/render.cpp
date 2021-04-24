/*
 ____  _____ _        _
| __ )| ____| |      / \
|  _ \|  _| | |     / _ \
| |_) | |___| |___ / ___ \
|____/|_____|_____/_/   \_\

The platform for ultra-low latency audio and sensor processing

http://bela.io

A project of the Augmented Instruments Laboratory within the
Centre for Digital Music at Queen Mary University of London.
http://www.eecs.qmul.ac.uk/~andrewm

(c) 2016 Augmented Instruments Laboratory: Andrew McPherson,
  Astrid Bin, Liam Donovan, Christian Heinrichs, Robert Jack,
  Giulio Moro, Laurel Pardue, Victor Zappi. All rights reserved.

The Bela software is distributed under the GNU Lesser General Public License
(LGPL 3.0), available here: https://www.gnu.org/licenses/lgpl-3.0.txt
*/


#include <Bela.h>
#include <libraries/Midi/Midi.h>

float gSamplingPeriod = 0;
int gSampleCount = 44100; // how often to send out a control change

int gAudioFramesPerAnalogFrame;

int gIn1 = 0;
int gIn2 = 1;
int gIn3 = 2;
int gIn4 = 3;
int gIn5 = 4;
int gIn6 = 5;
int gIn7 = 6;
Midi midi;

const char* gMidiPort0 = "hw:0,0";

bool setup(BelaContext *context, void *userData)
{
	midi.writeTo(gMidiPort0);
	gSamplingPeriod = 1 / context->audioSampleRate;
	
	if(context->analogFrames == 0 || context->analogFrames > context->audioFrames) {
        rt_printf("Error: this example needs analog enabled, with 4 or 8 channels\n");
        return false;
	}
        // Useful calculations
	if(context->analogFrames)
	gAudioFramesPerAnalogFrame = context->audioFrames / context->analogFrames;
	return true;
	
}

enum {kVelocity, kNoteOn, kNoteNumber};
void render(BelaContext *context, void *userData)
{
	int ctl1, ctl2, ctl3;
	float flex1, flex2, flex3, flex4;
	float accelx, accely, accelz;
	float flexavg, flexavg2;
	float accelavg;
	
	
	for(unsigned int i = 0; i < context->audioFrames; i++) {

		if(gAudioFramesPerAnalogFrame && !(i % gAudioFramesPerAnalogFrame)) {
			// read analog inputs and update frequency and amplitude
			flex1 = analogRead(context, i/gAudioFramesPerAnalogFrame, gIn1);
			flex2 = analogRead(context, i/gAudioFramesPerAnalogFrame, gIn2);
			flex3 = analogRead(context, i/gAudioFramesPerAnalogFrame, gIn4);
			flex4 = analogRead(context, i/gAudioFramesPerAnalogFrame, gIn3);
			
			accelx = analogRead(context, i/gAudioFramesPerAnalogFrame, gIn5);
			accely = analogRead(context, i/gAudioFramesPerAnalogFrame, gIn6);
			accelz = analogRead(context, i/gAudioFramesPerAnalogFrame, gIn7);
			

            flexavg = flex1+flex2;
			flexavg2 = flex3+flex4;
			
			accelavg = accelx+accely+accelz;
			
			if(flexavg > 1.8)
			{
				flexavg = 1.8;
			}
			else if(flexavg < 1.4)
			{
				flexavg = 1.4;
			}
			
			if(accelavg>2)
			{
				accelavg = 2;
			}
			
			ctl1 = map(flexavg, 1.4, 1.8, 127, 0);
			ctl2 = map(flexavg2, 0, 1, 0, 127);
            ctl3 = map(accelavg, 1.5, 2, 0, 127);
            
		}
	
	
			int count;
			
			if(count % gSampleCount == 0)
			{
			midi_byte_t statusByte = 0xB0; // control change on channel 0
			midi_byte_t controller = 30; // controller number 30
			midi_byte_t value = ctl1; // value : 0 or 127
			midi_byte_t bytes[3] = {statusByte, controller, value};
			midi.writeOutput(bytes, 3);
			
			midi_byte_t statusByte2 = 0xB0; // control change on channel 0
			midi_byte_t controller2 = 31; // controller number 30
			midi_byte_t value2 = ctl2; // value : 0 or 127
			midi_byte_t bytes2[3] = {statusByte2, controller2, value2};
			midi.writeOutput(bytes2, 3);
			
			midi_byte_t statusByte3 = 0xB0; // control change on channel 0
			midi_byte_t controller3 = 32; // controller number 30
			midi_byte_t value3 = ctl3; // value : 0 or 127
			midi_byte_t bytes3[3] = {statusByte3, controller3, value3};
			midi.writeOutput(bytes3, 3);
			}
			count++;
// send a control change message

		
	}
}

void cleanup(BelaContext *context, void *userData)
{
	
}

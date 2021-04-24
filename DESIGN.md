# Bela Glove
## Design Document

### Introduction

Bela Glove is a MIDI glove controller using the Bela platform.

### Demonstration video *[here](https://drive.google.com/file/d/1N1A_rQIbAzSMCw04MAwtt7tbjtdASMgG/view?usp=sharing)*

### Sensor Design

The glove has four flex sensors that act as variable resistors, going in to the analog input pins 0 through 3 

In addition, it also includes an accelerometer that measures the orientation of the glove. The accelerometer measures X, Y, and Z position and is also plugged in to the analog inputs 4 through 7.

### Performance Implementation

The data gathered from the analog ins is mapped to MIDI ranges 0-127, and are sent as continuous controllers in to max. The max patch displayed in this demo is a risset quantizer where the first two flex sensors control rate. 

The third and fourth flex sensors are used to control amplitude and filter cutoff, and the accelerometer adjusts the filter resonance.

### Future steps

While, I had originally anticipated designing a synthesizer specifically for the glove within Bela, I encountered difficulties in scaling the appropriate analog rate and audio rate. 

Ultimately, I’m satisfied with the synergy of interactive movement and sound, and believe this controller can be a start to new performance ideas and artistic directions.

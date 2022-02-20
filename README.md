# teensymod
code and build instructions for easypeesy teensy

# introduction
this is nothing more than teensyaudio shield + teensy 3.2 and some potentiometers. This is specifically so you can build this with parts you hgave lying around and a teensy + audioshield.

# BOM
- 4* 3.5mm mono jacks
- 3* 10K pot linear (anything in range 10K-100K will do
- 2* button

# pins
I did not want to make an extra PCB, so soldering is directly on the audio shield... you better use extra thin wires

- the 4 jacks got to the line-in and line-out section
- the pots go to 16,17,20 (A2,A3,A6)
- the buttons go to 4,5

![image](https://user-images.githubusercontent.com/204628/154855260-25097839-afc6-4f77-baa1-b5f52ecab09c.png)
The 3.3V pin is used to give ref voltage to the pots, don't use 5V!

![image](https://user-images.githubusercontent.com/204628/154855303-4c17d5dd-b43a-4ef8-a79f-5ef4e7e47546.png)


# power
The power cable is perfectly compatible with Eurorack, but instead of adding a connector I soldered it directly on the audioshield. +5v goes to Vin on the teensy, don't use 12V! The teensy is only rated for 6V max. GND > GND

# software
Currently 3 patches are available:

## template
test the hardware, usefull for debugging and as a starting point to explore teensyaudio

## granverb 
![granverb code](./granverb)  
Yeah.. granulator + freeverb  
left channel is granulator  
right channel is freeverb

you can of course chain them!

## WaveFormModulator
![WaveFormModulator code](./WaveformsModulated)
taken from example, repurposed for this specific setup

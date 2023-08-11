#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            audioInput;     //xy=153,289
AudioMixer4              mixer1;         //xy=436,294
AudioEffectFreeverb      freeverb1;      //xy=636,357
AudioEffectBitcrusher    bitcrusher1;    //xy=646,250
AudioMixer4              mixer2; //xy=811,275
AudioMixer4              mixer3; //xy=828,366
AudioAmplifier           amp1;           //xy=1011,272
AudioAmplifier           amp2;           //xy=1014,355
AudioOutputI2S           i2s1;           //xy=1199,314
AudioConnection          patchCord1(audioInput, 0, mixer1, 0);
AudioConnection          patchCord2(audioInput, 1, mixer1, 1);
AudioConnection          patchCord3(mixer1, bitcrusher1);
AudioConnection          patchCord4(mixer1, 0, mixer2, 0);
AudioConnection          patchCord5(mixer1, 0, mixer3, 0);
AudioConnection          patchCord6(mixer1, freeverb1);
AudioConnection          patchCord7(freeverb1, 0, mixer3, 1);
AudioConnection          patchCord8(bitcrusher1, 0, mixer2, 1);
AudioConnection          patchCord9(mixer2, amp1);
AudioConnection          patchCord10(mixer3, amp2);
AudioConnection          patchCord11(amp1, 0, i2s1, 0);
AudioConnection          patchCord12(amp2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=243,202
// GUItool: end automatically generated code





const int myInput = AUDIO_INPUT_LINEIN;
Bounce button0 = Bounce(4, 30);
Bounce button1 = Bounce(5, 30); //wait 30msec four bounce tot settle

int verb = 0;
int crush = 0;
int count = 0;



void setup() {
    Serial.begin(38400); // open the serial

  // put your setup code here, to run once:
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  AudioMemory(18);
    // Comment these out if not using the audio adaptor board.
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.volume(0.8); // caution: very loud - use oscilloscope only!
  sgtl5000_1.lineOutLevel(13); //13 loudest, 31 quietest
  sgtl5000_1.lineInLevel(6); //0 least sensitive, 15 most sensitive
}

void loop() {
 count++;
  // put your main code here, to run repeatedly:
  button0.update();
  button1.update();
  float knob_A2 = (float)analogRead(A2) / 1023.0;
  float knob_A3 = (float)analogRead(A3) / 1023.0;
  float knob_A6 = (float)analogRead(A6) / 1023.0;
  if (count%100 == 0) {
   Serial.println("knobs");
 Serial.println(knob_A2);
  Serial.println(knob_A3);
   Serial.println(knob_A6);
  }
  mixer1.gain(0, 1);  
  mixer1.gain(1, 1);
  amp1.gain(knob_A3);
  amp2.gain(1-knob_A3);
    if (button0.fallingEdge()) {
      if (verb == 1) {
          mixer3.gain(0, 1); 
          mixer3.gain(1, 0);
          verb = 0;
        } else {
          verb = 1;
          mixer3.gain(1, 1); 
          mixer3.gain(0, 0);
        }
      }
    if (button1.fallingEdge()) {
      if (crush == 1) {
          mixer2.gain(0, 1); 
          mixer2.gain(1, 0);
          crush = 0;
        } else {
          crush = 1;
          mixer2.gain(1, 1); 
          mixer2.gain(0, 0);
        }
      }

  freeverb1.roomsize(knob_A2);
  freeverb1.damping(knob_A6);
  int crushbits = int(knob_A2*14) + 2;
  int crushfreq = 44100/(int(knob_A6*8) + 2);
  bitcrusher1.bits(crushbits);
  bitcrusher1.sampleRate(crushfreq);
}

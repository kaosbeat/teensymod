#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Bounce.h>
// GUItool: begin automatically generated code
AudioInputI2S            audioInput;     //xy=108.5,232
AudioEffectGranular      granular1;      //xy=304.33331298828125,195.33331298828125
AudioEffectFreeverb      freeverb1;      //xy=308.33331298828125,231.33331298828125
AudioOutputI2S           i2s1;           //xy=697.3333129882812,210
AudioConnection          patchCord1(audioInput, 0, granular1, 0);
AudioConnection          patchCord2(audioInput, 1, freeverb1, 0);
AudioConnection          patchCord3(granular1, 0, i2s1, 0);
AudioConnection          patchCord4(freeverb1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=295.33331298828125,451.0000305175781
// GUItool: end automatically generated code



#define GRANULAR_MEMORY_SIZE 12000 // 12800  // enough for 290 ms at 44.1 kHz
int16_t granularMemory[GRANULAR_MEMORY_SIZE];
const int myInput = AUDIO_INPUT_LINEIN;
Bounce button0 = Bounce(4, 30);
Bounce button1 = Bounce(5, 30); //wait 30msec four bounce tot settle
//Bounce button2 = Bounce(2, 15);

int granfreezestate = 0;


void setup() {
  // put your setup code here, to run once:
  //  Serial.begin(9600);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  //  pinMode(2, INPUT_PULLUP);

  //  delay(300);
  //  Serial.println("Waveform Modulation Test");

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(18);

  // Comment these out if not using the audio adaptor board.
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.volume(0.8); // caution: very loud - use oscilloscope only!
  sgtl5000_1.lineOutLevel(13); //13 loudest, 31 quietest
  sgtl5000_1.lineInLevel(6); //0 least sensitive, 15 most sensitive
  // Confirgure both to use "myWaveform" for WAVEFORM_ARBITRARY
  // the Granular effect requires memory to operate
  granular1.begin(granularMemory, GRANULAR_MEMORY_SIZE);
}
int count = 0;
void loop() {
  // put your main code here, to run repeatedly:
  button0.update();
  button1.update();
  //  button2.update();
  float knob_A2 = (float)analogRead(A2) / 1023.0;
  float knob_A3 = (float)analogRead(A3) / 1023.0;
  float knob_A6 = (float)analogRead(A6) / 1023.0;

  freeverb1.roomsize(knob_A2);
  freeverb1.damping(knob_A3);

  //  bitcrusher1.bits(int(knob_A6*16));




/////Granular code

  // Button 0 starts Freeze effect
  if (button0.fallingEdge()) {
    if (granfreezestate == 1) {
      //      Serial.println("stopfreeze");
      granular1.stop();
      granfreezestate = 0;
    } else {
      //      Serial.println("startfreeze");
      granfreezestate = 1;
      float msec = 100.0 + (knob_A6 * 172.0);
      granular1.beginFreeze(msec);
    }
  }
  if (button1.fallingEdge()) {
    if (granfreezestate == 2) {
      //      Serial.println("stoppitch");
      granular1.stop();
      granfreezestate = 0;
    } else {
      //      Serial.println("startpitch");
      granfreezestate = 2;
      float msec = 25.0 + (knob_A6 * 75.0);
      granular1.beginPitchShift(msec);
    }
  }


  // Continuously adjust the speed, based on the A6 pot
  float ratio;
  //  ratio = powf(2.0, knob_A6 * 2.0 - 1.0); // 0.5 to 2.0
  ratio = powf(2.0, knob_A6 * 6.0 - 3.0); // 0.125 to 8.0 -- uncomment for far too much range!
  granular1.setSpeed(ratio);


//  count++;
//  if (count > 1500) {
//    //    Serial.print(String(knob_A2) + " " + String(knob_A3) + " " + String(knob_A6));
//    Serial.println("knobs");
//    Serial.println(knob_A2 );
//    Serial.println(knob_A3 );
//    Serial.println(knob_A6 );
//    count = 0;
//  }

}

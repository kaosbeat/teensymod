// Waveform Modulation Example - Create waveforms with
// modulated frequency
//
// This example is meant to be used with 3 buttons (pin 0,
// 1, 2) and 2 knobs (pins 16/A2, 17/A3), which are present
// on the audio tutorial kit.
//   https://www.pjrc.com/store/audio_tutorial_kit.html
//
// Use an oscilloscope to view the 2 waveforms.
//
// Button0 changes the waveform shape
//
// Knob A2 changes the amount of frequency modulation
//
// Knob A3 varies the shape (only for Pulse & Variable Triangle)
//
// This example code is in the public domain.

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>


// GUItool: begin automatically generated code
AudioInputI2S            audioInput;     //xy=62.5,201
AudioAnalyzeRMS          rms_L;          //xy=221.3333282470703,111.99999237060547
AudioAnalyzeRMS          rms_R;          //xy=221.3333282470703,150
AudioSynthWaveformSine   sine1;          //xy=279.3333282470703,307.00000762939453
AudioSynthWaveformSine   sine2;          //xy=282.33331298828125,345
AudioAnalyzePeak         peak_L;         //xy=353.33331298828125,147
AudioAnalyzePeak         peak_R;         //xy=357.33331298828125,107.99999237060547
AudioSynthWaveformModulated waveformMod1;   //xy=461.33331298828125,270
AudioEffectFreeverb      freeverb1;      //xy=564.3333129882812,336.33331298828125
AudioOutputI2S           i2s1;           //xy=709.3333129882812,327
AudioConnection          patchCord1(audioInput, 0, peak_L, 0);
AudioConnection          patchCord2(audioInput, 0, rms_L, 0);
AudioConnection          patchCord3(audioInput, 1, peak_R, 0);
AudioConnection          patchCord4(audioInput, 1, rms_R, 0);
AudioConnection          patchCord5(sine1, 0, waveformMod1, 0);
AudioConnection          patchCord6(sine2, 0, waveformMod1, 1);
AudioConnection          patchCord7(waveformMod1, 0, i2s1, 0);
AudioConnection          patchCord8(waveformMod1, freeverb1);
AudioConnection          patchCord9(freeverb1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=295.33331298828125,451.0000305175781
// GUItool: end automatically generated code


Bounce button0 = Bounce(4, 15);
Bounce button1 = Bounce(5, 15);
//Bounce button2 = Bounce(2, 15);

int current_waveform = 0;
int audio_follow = 0;
elapsedMillis fps;
uint8_t cnt = 0;
extern const int16_t myWaveform[256];  // defined in myWaveform.ino
const int myInput = AUDIO_INPUT_LINEIN;
uint8_t leftPeak = peak_L.read();
uint8_t rightPeak = peak_R.read();
uint8_t leftRMS = rms_L.read();
uint8_t rightRMS = rms_R.read();


void setup() {
  Serial.begin(9600);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  //  pinMode(2, INPUT_PULLUP);

  //  delay(300);
  //  Serial.println("Waveform Modulation Test");

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(28);

  // Comment these out if not using the audio adaptor board.
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.volume(0.8); // caution: very loud - use oscilloscope only!
  sgtl5000_1.lineOutLevel(13); //13 loudest, 31 quietest
  sgtl5000_1.lineInLevel(12); //0 least sensitive, 15 most sensitive
  // Confirgure both to use "myWaveform" for WAVEFORM_ARBITRARY
  waveformMod1.arbitraryWaveform(myWaveform, 172.0);

  // Configure for middle C note without modulation
  waveformMod1.frequency(261.63);
  waveformMod1.amplitude(1.0);
  sine1.frequency(20.3); // Sine waves are low frequency oscillators (LFO)
  sine2.frequency(1.2);

  current_waveform = WAVEFORM_TRIANGLE_VARIABLE;
  waveformMod1.begin(current_waveform);

  // uncomment to try modulating phase instead of frequency
  //waveformMod1.phaseModulation(720.0);
}

void loop() {
  // Read the buttons and knobs, scale knobs to 0-1.0
  button0.update();
  button1.update();
  //  button2.update();
  float knob_A2 = (float)analogRead(A2) / 1023.0;
  float knob_A3 = (float)analogRead(A3) / 1023.0;
  float knob_A6 = (float)analogRead(A6) / 1023.0;
  freeverb1.roomsize(knob_A2);
  freeverb1.damping(knob_A3);
  // use Knobsto adjust the amount of modulation

  //  Serial.println(fps);
  if (fps > 24) {
    if (peak_L.available() && peak_R.available() && rms_L.available() && rms_R.available()) {
      fps = 0;
      leftPeak = peak_L.read() * 30;
      rightPeak = peak_R.read() * 30;
      leftRMS = rms_L.read() * 30;
      rightRMS = rms_R.read() * 30;
      Serial.println(String(leftPeak) + " " + String(rightPeak) + " " + String(leftRMS) + " " + String(rightRMS));
      Serial.println(String(knob_A2) + " " + String(knob_A3) + " " + String(knob_A6));

    }
  }

  if (audio_follow == 1) {
    sine1.amplitude(leftPeak);
    sine2.amplitude(rightPeak);
  } else {
    sine1.amplitude(knob_A2);
    sine2.amplitude(knob_A3);
  }
  //
  sine1.frequency(knob_A6 * 25 + 20); // Sine waves are low frequency oscillators (LFO)
  sine2.frequency(knob_A3 * 512);

  if (button1.fallingEdge()) {
    if (audio_follow == 0) {
      audio_follow = 1;
    } else {
      audio_follow = 0;
    }

  }

  // Button 0
  if (button0.fallingEdge()) {
    switch (current_waveform) {
      case WAVEFORM_SINE:
        current_waveform = WAVEFORM_SAWTOOTH;
        Serial.println("Sawtooth");
        break;
      case WAVEFORM_SAWTOOTH:
        current_waveform = WAVEFORM_SAWTOOTH_REVERSE;
        Serial.println("Reverse Sawtooth");
        break;
      case WAVEFORM_SAWTOOTH_REVERSE:
        current_waveform = WAVEFORM_SQUARE;
        Serial.println("Square");
        break;
      case WAVEFORM_SQUARE:
        current_waveform = WAVEFORM_TRIANGLE;
        Serial.println("Triangle");
        break;
      case WAVEFORM_TRIANGLE:
        current_waveform = WAVEFORM_TRIANGLE_VARIABLE;
        Serial.println("Variable Triangle");
        break;
      case WAVEFORM_TRIANGLE_VARIABLE:
        current_waveform = WAVEFORM_ARBITRARY;
        Serial.println("Arbitary Waveform");
        break;
      case WAVEFORM_ARBITRARY:
        current_waveform = WAVEFORM_PULSE;
        Serial.println("Pulse");
        break;
      case WAVEFORM_PULSE:
        current_waveform = WAVEFORM_SAMPLE_HOLD;
        Serial.println("Sample & Hold");
        break;
      case WAVEFORM_SAMPLE_HOLD:
        current_waveform = WAVEFORM_SINE;
        Serial.println("Sine");
        break;
    }
    waveformMod1.begin(current_waveform);
    //    waveformMod1.frequency(knob_A6 + 261.63);
  }



}

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
//#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>


// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=331.8833312988281,475.88330078125
AudioOutputI2S           i2s1;           //xy=603.88330078125,471.0833435058594
AudioConnection          patchCord1(i2s2, 0, i2s1, 0);
AudioConnection          patchCord2(i2s2, 1, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=480.8833312988281,555.0833129882812
// GUItool: end automatically generated code
// init buttons
Bounce button0 = Bounce(4, 15); // 15ms bounce time
Bounce button1 = Bounce(5, 15);

//init line in
const int myInput = AUDIO_INPUT_LINEIN;

int printdelay = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

  AudioMemory(12);
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.volume(0.8); // caution: very loud
  sgtl5000_1.lineOutLevel(13); //13 loudest, 31 quietest
  sgtl5000_1.lineInLevel(12); //0 least sensitive, 15 most sensitive
  // Confirgure both to use "
}

void loop() {
  // put your main code here, to run repeatedly:
  printdelay++;
  button0.update();
  button1.update();
  float knob_A2 = (float)analogRead(A2) / 1023.0;
  float knob_A3 = (float)analogRead(A3) / 1023.0;
  float knob_A6 = (float)analogRead(A6) / 1023.0;
  if (printdelay > 15000) {
    Serial.println(String(knob_A2) + " " + String(knob_A3) + " " + String(knob_A6));
    printdelay = 0;
  }
  if (button0.fallingEdge()) {
    Serial.println("button0 pressed");
  }
  if (button1.fallingEdge()) {
    Serial.println("button1 pressed");
  }

}

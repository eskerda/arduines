/* Author: eskerda
 * Released into the wild
 */

int AXIS_VAL = 100;

int clockPin = 2;
int latchPin = 3;
int dataPin  = 4;
byte lastInput = 255;

void setup();
void loop();

void setup() 
{
    Serial.begin(115200);
    pinMode(clockPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, INPUT);
}

void loop() 
{
    byte data = readNESController();
    if (data != lastInput){
         Serial.println(data, BIN);
    }
    lastInput = data;
}

byte readNESController() {
      /*
        NES Word Mapping
        x x x x x x x x
        | | | | | | | |_  A
        | | | | | | |___  B
        | | | | | |_____  SELECT
        | | | | |_______  START
        | | | |_________  UP
        | | |___________  DOWN
        | |_____________  LEFT
        |_______________  RIGHT
     */

  // Send a HIGH pulse to latch. Make 8 shift register store state
  // of all buttons
  digitalWrite(latchPin, HIGH);
  delayMicroseconds(200);
  digitalWrite(latchPin, LOW);
  
  // Clock the 8 shift register to get the
  // state of the buttons
  byte output = 0x00;
  for (int i = 0; i < 8; i++){
    output = output | !digitalRead(dataPin) << i;
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(200);
    digitalWrite(clockPin, LOW);
  }
  return output;
}


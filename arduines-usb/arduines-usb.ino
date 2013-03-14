/* Arduino USB Joystick HID demo */

/* Author: Darran Hunt
 * Released into the public domain.
 */
 
/* Author: eskerda
 * Released into the wild
 */

int AXIS_VAL = 100;

int clockPin = 2;
int latchPin = 3;
int dataPin  = 4;

byte lastInput = 0;

struct {
    int8_t x;
    int8_t y;
    uint8_t buttons;
    uint8_t rfu; 	/* reserved for future use */
} joyReport;

void setup();
void loop();

void setup() 
{
    Serial.begin(115200);
    pinMode(clockPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, INPUT);
    delay(1000);
}

void loop() 
{
    joyReport.buttons = 0;
    joyReport.x = 0;
    joyReport.y = 0;
    joyReport.rfu = 0;

    byte data = readNESController();
    if (data != lastInput) {
      joyReport.buttons = data & 0x0F;
      byte directions = (data & 0xF0) >> 4;
      joyReport.y = (directions & 0x01) * -AXIS_VAL;
      joyReport.y = joyReport.y + ((directions & 0x02) >> 1) * AXIS_VAL;
      joyReport.x = ((directions & 0x04) >> 2) * -AXIS_VAL;
      joyReport.x = joyReport.x + ((directions & 0x08) >> 3) * AXIS_VAL;
      Serial.write((uint8_t *)&joyReport, 4);
      /***
      Serial.print("x: ");
      Serial.print(joyReport.x);
      Serial.print("y: ");
      Serial.print(joyReport.y);
      Serial.println(joyReport.buttons, BIN);
      ***/
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


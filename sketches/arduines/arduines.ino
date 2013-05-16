/* Based on Arduino USB Joystick HID demo
 * Author: Darran Hunt
 * 
 * https://github.com/harlequin-tech/arduino-usb
 */

int AXIS_VAL = 1;

int CLK   = 2;
int LATCH = 3;
int DATA  = 4;

byte last_input = 0;

struct {
    int8_t x;
    int8_t y;
    uint8_t buttons;
} hid_report;

void setup();
void loop();

void setup() 
{
    Serial.begin(115200);
    pinMode(CLK, OUTPUT);
    pinMode(LATCH, OUTPUT);
    pinMode(DATA, INPUT);
    delay(1000);
}

void loop() 
{
    byte reading = read_NESpad();

    if (reading != last_input) {
        byte directions = (reading & 0xF0) >> 4;
        hid_report.buttons = reading & 0x0F;

        hid_report.y = (directions & 0x01) * -AXIS_VAL;
        hid_report.y += ((directions & 0x02) >> 1) * AXIS_VAL;
        hid_report.x = ((directions & 0x04) >> 2) * -AXIS_VAL;
        hid_report.x += ((directions & 0x08) >> 3) * AXIS_VAL;
        
        Serial.write((uint8_t *) &hid_report, 4);
    }

    last_input = reading;
}

byte read_NESpad() {
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
    digitalWrite(LATCH, HIGH);
    digitalWrite(LATCH, LOW);

    // Clock the 8 shift register to get the
    // state of the buttons
    byte output = 0x00;
    for (int i = 0; i < 8; i++) {
        output |= !digitalRead(DATA) << i;
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
    }
    return output;
}

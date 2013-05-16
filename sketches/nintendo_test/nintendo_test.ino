int CLK   = 2;
int LATCH = 3;
int DATA  = 4;

byte last_input = 0;

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
    
    if (reading != last_input)
         Serial.println(reading, BIN);

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

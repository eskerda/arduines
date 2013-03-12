int clockPin = 2;
int latchPin = 3;
int dataPin  = 4;

byte lastOut = 255;

void setup() {
  Serial.begin(9600);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, INPUT);
  Keyboard.begin();
}

void loop() {
  byte data = readNESController();
  if (data != lastOut){
    Serial.write(data);
  }
  lastOut = data;
  delayMicroseconds(200);
}

byte readNESController() {
  // Send a HIGH pulse to latch. Make 8 shift register store state
  // of all buttons
  digitalWrite(latchPin, HIGH);
  delayMicroseconds(200);
  digitalWrite(latchPin, LOW);
  
  // Clock the 8 shift register to get the
  // state of the buttons
  byte output = 0x00;
  for (int i = 0; i < 8; i++){
    output = output | digitalRead(dataPin) << i;
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(200);
    digitalWrite(clockPin, LOW);
  }
  return output;
}

const int resetPin = 3;
const int powerPin = 4;
const int holdTimeConsole = 350;
const int bootTime = 15000;
void setup() {
  digitalWrite(8, HIGH); // set up solenoid driver
  pinMode(resetPin, OUTPUT);
  pinMode(powerPin, OUTPUT);  
  //////
  delay(5000);
  digitalWrite(resetPin, HIGH);
  delay(6000);                  // hold mac power button to shut down
  digitalWrite(resetPin, LOW);
  //////
  delay(3000);
  //////
  digitalWrite(resetPin, HIGH);
  delay(500);                  // turn on mac
  digitalWrite(resetPin, LOW);
  //////
  delay(bootTime);              // wait for mac to power on
  //////
  digitalWrite(powerPin, HIGH);
  delay(1000);
  digitalWrite(powerPin, LOW);
  delay(holdTimeConsole);       // turn on console
  digitalWrite(powerPin, HIGH);
}
void loop() {
}


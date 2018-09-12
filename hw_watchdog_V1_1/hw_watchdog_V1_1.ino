#include "MIDIUSB.h"

bool isAlive;
bool isOn;
bool wasOn;
bool active;
const int statusLED = 6;
const int blinker = 13;
const int maxLED = 70;
const int optoIn = 2;
const int deadTime = 1000;

void setup() {
  pinMode(statusLED, OUTPUT);
  pinMode(blinker, OUTPUT);
  pinMode(optoIn, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  isAlive = false;
  isOn = false;
  wasOn = false;
  active = digitalRead(8);
}


void loop() {
  if (active) {
    if (isAlive) {
      sendMidi();
      MidiUSB.flush();
      delay(100);
      if (!isOn) {
        if (wasOn) {
          fadeOff(2500);
        }
        pulse();
        wasOn = false;
      } else {
        if (!wasOn) {
          //sendMidiOn();
          fadeOn(2500);
          wasOn = true;
        }
        analogWrite(blinker, maxLED);
        delay(1500);
      }
      checkIfDead();
    } else {
      digitalWrite(statusLED, LOW);
      sendMidi();
      MidiUSB.flush();
      delay(deadTime);
      checkIfDead();
    }
    isOn = !digitalRead(optoIn);
  } else {
    digitalWrite(blinker, LOW);
    digitalWrite(statusLED, LOW);
  }
  active = digitalRead(8);
}

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void sendMidi() {
  noteOn(0,60,127);
  MidiUSB.flush();
  delay(50);
  noteOff(0,60,0);
  MidiUSB.flush();
}

void sendMidiOn() {
  noteOn(0,70,127);
  MidiUSB.flush();
  delay(50);
  noteOff(0,70,0);
  MidiUSB.flush();
}

void checkIfDead() {
  midiEventPacket_t rx;
  rx = MidiUSB.read();
  if (rx.header != 0) {
    isAlive = true;
  } else {
    isAlive = false;
  }
  MidiUSB.flush();
  if (!isAlive) {
    if (wasOn) {
      fadeOff(1500);
      wasOn = false;
    }
    digitalWrite(statusLED, HIGH);
    delay(deadTime);
  } else {
    digitalWrite(statusLED, LOW);
  }
  while (rx.header != 0) {
    MidiUSB.flush();
    rx = MidiUSB.read();
  }
}


void pulse() {
  fadeOn(2000);
  fadeOff(2000);
  delay(800);
}
bool checkIfOn() {
  int i;
  if (digitalRead(optoIn) == 0) {
    return true;
  }
   return false;
}

//void fadeOn(int ms) {
//  int level;
//  for (int i = 0; i < maxLED; i++) {
//    level = log(i + 1)*maxLED/log(maxLED);
//    analogWrite(blinker, level);
//    delay(ms/maxLED);
//  }
//}
//void fadeOff(int ms) {
//  int level;
//  for (int i = maxLED; i >= 0; i--) {
//    level = log(i + 1)*maxLED/log(maxLED);
//    analogWrite(blinker, level);
//    delay(ms/maxLED);
//  }
//}

void fadeOn(int ms) {
  int level;
  for (int i = 0; i < maxLED; i++) {
    analogWrite(blinker, i);
    delay(ms/maxLED);
  }
}
void fadeOff(int ms) {
  int level;
  for (int i = maxLED; i >= 0; i--) {
    analogWrite(blinker, i);
    delay(ms/maxLED);
  }
}

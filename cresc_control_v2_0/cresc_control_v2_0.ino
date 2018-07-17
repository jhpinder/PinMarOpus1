#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

const byte oneLed = 0x01;
const byte twoLeds = 0x02;
const byte threeLeds = 0x04;
const byte fourLeds = 0x0c;
const byte fiveLeds = 0x14;
const byte sixLeds = 0x24;
const byte sevenLeds = 0x44;
const byte eightLeds = 0xc4;
const byte justRed = 0x80;

const int clk = 11;
const int data = 12;
const int str = 13;
const int del = 90;
const int piston = 3;

int curr;
int last;
int cresc = 0;
bool tutti = false;

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.turnThruOff();

  pinMode(piston, INPUT_PULLUP);
  pinMode(clk, OUTPUT);
  digitalWrite(clk, HIGH);
  pinMode(data, OUTPUT);
  digitalWrite(data, HIGH);
  pinMode(str, OUTPUT);
  digitalWrite(str, HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(A0, OUTPUT);
  analogWrite(A0, 0);

  curr = digitalRead(piston);
  last = curr;

  delay(2000);
  shiftMe(0x00);
  delay(del);
  shiftMe(0x01);
  delay(del);
  shiftMe(0x02);
  delay(del);
  shiftMe(0x04);
  delay(del);
  shiftMe(0x0c);
  delay(del);
  shiftMe(0x14);
  delay(del);
  shiftMe(0x24);
  delay(del);
  shiftMe(0x44);
  delay(del);
  shiftMe(0xc4);
  delay(200);
  
  shiftMe(0x44);
  delay(del);
  shiftMe(0x24);
  delay(del);
  shiftMe(0x14);
  delay(del);
  shiftMe(0x0c);
  delay(del);
  shiftMe(0x04);
  delay(del);
  shiftMe(0x02);
  delay(del);
  shiftMe(0x01);
  delay(del);
  shiftMe(0x00);
}

void loop() {
  // put your main code here, to run repeatedly:
  curr = digitalRead(piston);
  if (curr != last) {
    if (curr == HIGH) {
      MIDI.sendNoteOff(36, 0, 1);
    } else {
      MIDI.sendNoteOn(36, 127, 1);
    }
  }
  last = curr;
  updateCresc(tutti, cresc);
  MIDI.read();
  delay(3);
}

void shiftMe(byte toShift) {
  for (int i = 7; i >= 0; i--) {
    writeBit((toShift & 0x80) == 0x80);
    toShift = toShift << 1;
  }
  digitalWrite(str, LOW);
  digitalWrite(str, HIGH);
}

void writeBit(bool toWrite) {
  if (toWrite) {
    digitalWrite(data, LOW);
  } else {
    digitalWrite(data, HIGH);
  }
  digitalWrite(clk, LOW);
  digitalWrite(clk, HIGH);
}

void handleControlChange(byte channel, byte number, byte value) {
//  if (channel != 0x01) {
//    return;
//  }
//  if (number != 0x0C) {
//    return;
//  }
  
  if (value < 0x03) {
    cresc = 0;
  } else if (value < 0x10) {
    cresc = 1;
  } else if (value < 0x20) {
    cresc = 2;
  } else if (value < 0x30) {
    cresc = 3;
  } else if (value < 0x40) {
    cresc = 4;
  } else if (value < 0x50) {
    cresc = 5;
  } else if (value < 0x60) {
    cresc = 6;
  } else if (value < 0x70) {
    cresc = 7;
  } else {
    cresc = 8;
  }
}

void handleNoteOn(byte channel, byte note, byte velocity) {
  if (note == 36) {
    tutti = true;
  }
}

void handleNoteOff(byte channel, byte note, byte velocity) {
  if (note == 36) {
    tutti = false;
  }  
}

void updateCresc(bool t, int c) {
  if (t) {
    shiftMe(eightLeds);
    return;
  }
  switch (cresc) {
      case 0:
        shiftMe(0x00);
        break;
      case 1:
        shiftMe(oneLed);
        break;
      case 2:
        shiftMe(twoLeds);
        break;
      case 3:
        shiftMe(threeLeds);
        break;
      case 4:
        shiftMe(fourLeds);
        break;
      case 5:
        shiftMe(fiveLeds);
        break;
      case 6:
        shiftMe(sixLeds);
        break;
      case 7:
        shiftMe(sevenLeds);
        break;
      case 8:
        shiftMe(eightLeds);
        break;
    }
}


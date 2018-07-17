#include "MIDIUSB.h"
uint8_t data0[39] = 0xF0, 0x7D, 0x01, 0x00, 0x00, 0x01, 0x20, 0x50, 0x69, 0x6E, 0x4D, 0x61, 0x72, 0x20, 0x4F, 0x72, 0x67, 0x61, 0x6E, 0x73, 0x20, 0x20, 0x4F, 0x70, 0x75, 0x73, 0x20, 0x49, 0x20, 0x4C, 0x6F, 0x61, 0x64, 0x69, 0x6E, 0x67, 0x20, 0x20, 0xF7};

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  MidiUSB_sendSysEx(data0, 39);
  MidiUSB.flush();
  delay(1000);
}

void MidiUSB_sendSysEx(const uint8_t *data, size_t size)
{
    if (data == NULL || size == 0) return;

    size_t midiDataSize = (size+2)/3*4;
    uint8_t midiData[midiDataSize];
    const uint8_t *d = data;
    uint8_t *p = midiData;
    size_t bytesRemaining = size;

    while (bytesRemaining > 0) {
        switch (bytesRemaining) {
        case 1:
            *p++ = 5;   // SysEx ends with following single byte
            *p++ = *d;
            *p++ = 0;
            *p = 0;
            bytesRemaining = 0;
            break;
        case 2:
            *p++ = 6;   // SysEx ends with following two bytes
            *p++ = *d++;
            *p++ = *d;
            *p = 0;
            bytesRemaining = 0;
            break;
        case 3:
            *p++ = 7;   // SysEx ends with following three bytes
            *p++ = *d++;
            *p++ = *d++;
            *p = *d;
            bytesRemaining = 0;
            break;
        default:
            *p++ = 4;   // SysEx starts or continues
            *p++ = *d++;
            *p++ = *d++;
            *p++ = *d++;
            bytesRemaining -= 3;
            break;
        }
    }
    MidiUSB.write(midiData, midiDataSize);
}

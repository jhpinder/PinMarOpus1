# Crescendo Controller for Hauptwerk Organ
## Pinout
0: MIDI RX

1: MIDI TX

2: unused

3: tutti switch +

4: MIDI +

5: MIDI -

11: clock

12: data

13: strobe

A0. tutti switch -

VIN. +12V power in

GND: ground power in

## MIDI Information
Receives all MIDI CC expression pedal (0x11) and displays the value from 0 to 127 on the LED indicator. Values below 0x03 will not trigger the first segment of the indicator. Values at or above 0x70 will trigger the four red LED segments along with all the other segments.

The tutti piston sends a MIDI note on (note number 0x24, velocity 0x7F, channel 0x01) when pressed, and a matching note off when released. When an identical note on message is received, all the LED segments turn on. When an identical note off message is received, the LED segments return to the state of the crescendo pedal. However, if the crescendo pedal is moved while the tutti indicator is on, the crescendo controller will update in the background to keep the indicator synchronized with Hauptwerk so the indicator returns to the correct position when the tutti is disengaged.

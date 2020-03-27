/**
 * Program up to 100 LEDs in a WS2801 strip 9 different patterns
 * 
 * Display the active pattern number (0–9) on a 1 digit 7-segment display.
 * Change the active pattern in response to a potentiometer, or input from an IR remote. 
 */

#include "IRremote.h"
#include <FastLED.h>
#include "ir_remote.h"

// LED strip configuration
#define LED_COUNT 23
#define LED_DATA_PIN 12

int program_active; // the running program
int program_displayed; // which currently displayed on 7-segment
int waitTime = 0; // for giving loop() delay instructions from program functions

IRrecv ir_receiver(RECEIVER_PIN);     // create instance of 'ir_receiver'
decode_results results;      // create instance of 'decode_results'
CRGB leds[LED_COUNT]; // 23 LED strip instance

byte seven_seg_segments[13] = {
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11100110, // 9
  B00000001, // .
  B01101100, // 11 because it goes to 11 hahahah
  B00000000  // all off
};

void setProgram(int program = 0) {
  program_active = program;
}

void setup() {
  pinMode(SHIFT_DATA_PIN, OUTPUT);
  pinMode(SHIFT_CLOCK_PIN, OUTPUT);
  pinMode(SHIFT_LATCH_PIN, OUTPUT);
  pinMode(RECEIVER_PIN, INPUT);
  pinMode(LED_DATA_PIN, OUTPUT);

  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, LED_COUNT);

  setProgram(12); // the 'nothing' program

  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 
  ir_receiver.enableIRIn(); // Start the receiver
}

void programNothing() {
  for (int i = 0; i < 23; i++) {
    leds[i] = CRGB::Black;
  }
  waitTime = 0;
}

void programWhite() {
  for (int i = 0; i < 23; i++) {
    leds[i] = CRGB::White;
  }
  waitTime = 0;
}

void programBlueChase() {
  static bool even = true;
  for (int i = 0; i < 23; i++) {
    if (even) {
      leds[i] = 0 == i % 2 ? CRGB::Blue : CRGB::Black;
    } else {
      leds[i] = 0 == i % 2 ? CRGB::Black :CRGB::Blue;
    }
  }
  waitTime = 500;
  even = ! even;
}

// Call program function keyed to program number input
void lightProgram(int program) {
  switch (program) {
    case 1: programWhite();     break;
    case 2: programBlueChase(); break;

    default: programNothing();  break; // including case 0
  }
  FastLED.show();
}

void loop() {
  // have we received an IR signal?
  if (ir_receiver.decode(&results)) {
    translateIR(results);
    lightProgram(program_active);
    ir_receiver.resume(); // receive the next value
  }
  
  if (waitTime) {
    delay(waitTime);
    lightProgram(program_active);
  }
}

void updateShiftRegister(int segment) {
  digitalWrite(SHIFT_LATCH_PIN, LOW);
  shiftOut(SHIFT_DATA_PIN, SHIFT_CLOCK_PIN, LSBFIRST, seven_seg_segments[segment]);
  digitalWrite(SHIFT_LATCH_PIN, HIGH);
}

void updateDisplay() {
  if (program_active == program_displayed) {
    return;
  }
  updateShiftRegister(program_active);
  program_displayed = program_active;
}

#include "IRremote.h"
#include "main.h"

// TODO rename
// TODO restructure to return a value rather than calling setProgram() and updateDisplay() directly
void translateIR(decode_results results) {
  bool meaningful_input = true;
  switch(results.value) {
    case 0xFF6897: setProgram(0);  break;
    case 0xFF30CF: setProgram(1);  break;
    case 0xFF18E7: setProgram(2);  break;
    case 0xFF7A85: setProgram(3);  break;
    case 0xFF10EF: setProgram(4);  break;
    case 0xFF38C7: setProgram(5);  break;
    case 0xFF5AA5: setProgram(6);  break;
    case 0xFF42BD: setProgram(7);  break;
    case 0xFF4AB5: setProgram(8);  break;
    case 0xFF52AD: setProgram(9);  break;
    case 0xFF906F: setProgram(11); break; // "UP" button
    
    default: meaningful_input = false;
  }

  if (meaningful_input) {
    updateDisplay();
  }
  
  // debugging only — remove to preserve memory later!
  switch(results.value) {
    case 0xFF6897: Serial.println("0"); break;
    case 0xFF30CF: Serial.println("1"); break;
    case 0xFF18E7: Serial.println("2"); break;
    case 0xFF7A85: Serial.println("3"); break;
    case 0xFF10EF: Serial.println("4"); break;
    case 0xFF38C7: Serial.println("5"); break;
    case 0xFF5AA5: Serial.println("6"); break;
    case 0xFF42BD: Serial.println("7"); break;
    case 0xFF4AB5: Serial.println("8"); break;
    case 0xFF52AD: Serial.println("9"); break;
    case 0xFF906F: Serial.println("UP");break;
  }
}

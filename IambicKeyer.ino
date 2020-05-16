// Keyboard - Version: Latest
#include <Keyboard.h>
int potPin = 9;

int ditPin = 2;
int dahPin = 4;

char keyer = 32;      
double spacing = 100; //timing in milliseconds. Time of one dit, range of 240 (1 wpm) to 48 (25 wpm. 120 (10 wpm), 100 (13 wpm), 80 (15 wpm)
boolean alpha = false;

void setup()
{
  pinMode(ditPin, INPUT_PULLUP);  
  pinMode(dahPin, INPUT_PULLUP);

  Keyboard.begin();
}
void loop() {
  while (digitalRead(ditPin) == LOW)
  {
    sendDit();      
    delay(spacing);
    if (digitalRead(dahPin) == LOW)
    {
      sendDah();
      delay(spacing);
    }
  }

  while (digitalRead(dahPin) == LOW) 
  {
    sendDah();
    delay(spacing);
    if (digitalRead(ditPin) == LOW)
    {
      sendDit();
      delay(spacing);
    }
  }
}

void sendDit() {
    Keyboard.press(keyer);
    delay(spacing);
    Keyboard.release(keyer);
}

void sendDah() {
    Keyboard.press(keyer);
    delay(spacing*3);
    Keyboard.release(keyer);
}

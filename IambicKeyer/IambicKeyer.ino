// Keyboard - Version: Latest
#include <Keyboard.h>

int ditPin = 2; //The Pin your dit line is connected to. Corresponds to the number printed on the board.
int dahPin = 4; //The Pin your dah line is connected to. Corresponds to the number printed on the board.
int sw = 6; //The Pin you connect to one side of the switch.

int RXled = 17; //17 is the Pro Micro's built in RX light. A value of HIGH means that the LED is off, used to indicate which mode you're in
int TXled = 30; //30 is the Pro Mciros's built in TX light. By default it flashes when ever you key. I found that annoying and turned it off. To reenable, comment last line in setup() below.

char enter = 32; //This is the ascii code of enter
char keyer = enter; //This is the key the keyer will press.
char listOfKeys[] = {enter, KEY_DOWN_ARROW};

int wpm = 13; //The default wpm the keyer operates at upon startup.
float spacing = (60.0/(50.0*(float)(wpm)))*1000.0; //This math works out the corresponding time in milliseconds of one dit.

void setup()
{
  pinMode(ditPin, INPUT_PULLUP); //Sets the dit pin to go LOW when connected
  pinMode(dahPin, INPUT_PULLUP); //Sets the dah pin to go LOW when connected
  pinMode(sw, INPUT_PULLUP); //Sets the switch pin to go LOW when connected
  
  pinMode(RXled, OUTPUT); //Sets the TXled pin to output
  Keyboard.begin(); //Needed to initialize keyboard output.

  pinMode(TXled, INPUT); //Comment this line to enable flashing whenever you key.
}

void(* resetFunc) (void) = 0; //declare reset function @ address 0, useful for reflashing the board w/o shorting Gnd and Rst.

void loop() 
{
//Key Mode
  if(digitalRead(sw) == HIGH) //When the switch is OFF
  {
    digitalWrite(17,HIGH); //This turns the LED off, because reasons. Denotes that the keyer is in Key Mode.
    while (digitalRead(ditPin) == LOW) //When the dit level makes contact
    {
      sendDit();      
      delay(spacing);
      if (digitalRead(dahPin) == LOW)//When the dah lever makes contact
      {
        sendDah();
        delay(spacing);
      }
    }
  
    while (digitalRead(dahPin) == LOW) //When the dah lever makes contact
    {
      sendDah();
      delay(spacing);
      if (digitalRead(ditPin) == LOW) //When the dit level makes contact
      {
        sendDit();
        delay(spacing);
      }
    }
  }
  
//WPM Edit Mode
  else if(digitalRead(sw) == LOW) 
  {
    digitalWrite(17, LOW); //This turns the LED on, because reasons. Denotes that the keyer is in Edit Mode.
    if(digitalRead(ditPin) == LOW && digitalRead(dahPin) != LOW) //Press the dit paddle ONLY to increase the wpm by 1. Have your cursor in a notepad or other text editor so you can see the wpm.
    {
      speedUp();
    }
  
    else if(digitalRead(dahPin) == LOW && digitalRead(ditPin) != LOW) //Press the dit paddle ONLY to decrease the wpm by 1. Have your cursor in a notepad or other text editor so you can see the wpm.
    {
      speedDown();
    }
    else if(digitalRead(ditPin) == LOW && digitalRead(dahPin) == LOW) //Hit both paddles, then hold the dit lever to toggle the key that is sent, or the dah lever to reset the board.
    {
      delay(500);
      if(digitalRead(dahPin) == LOW)
      {
        resetFunc();
      }
      else if(digitalRead(ditPin) == LOW)
      {
        toggleWord();
        delay(1000);
      }
    }
  }
}

void sendDit() { //The following functions just clean up the code, no need to alter them unless you want to make the Edit Mode repeat faster
    Keyboard.press(keyer);
    delay(spacing);
    Keyboard.release(keyer);
}

void sendDah() {
    Keyboard.press(keyer);
    delay(spacing*3);
    Keyboard.release(keyer);
}

void speedUp()
{
  wpm++;
  spacing = 60.0/(50.0*(float)(wpm))*1000.0;
  Keyboard.println(wpm);
  Keyboard.println(' ');
  delay(200); //Adjust this value to make altering the wpm faster. A lower value will make it harder to control precisely.
}

void speedDown()
{
  wpm--;
  spacing = 60.0/(50.0*(float)(wpm))*1000.0;
  Keyboard.println(wpm);
  Keyboard.println(' ');
  delay(200); //Adjust this value to make altering the wpm faster. A lower value will make it harder to control precisely.
}

void toggleWord()
{
  if(keyer == enter)
  {
    keyer = KEY_DOWN_ARROW;
    Keyboard.println("down");
  }
  else
  {
    keyer = enter;
    Keyboard.println("enter");
  }
}


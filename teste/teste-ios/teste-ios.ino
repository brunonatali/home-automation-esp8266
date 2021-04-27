/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/
#include <user_interface.h>;

int val = 0;
int dir = 1;

bool out1 = false;
bool out2 = false;

void IRAM_ATTR handleInterruptA() {
  out1 = !out1;
}
void IRAM_ATTR handleInterruptB() {
  out2 = !out2;
}

void setup() {
  //pinMode(0, OUTPUT); // pullup // out
  pinMode(1, OUTPUT); // TX     // out
  pinMode(3, OUTPUT);  // RX     // in

  // starts swith off
  digitalWrite(1, HIGH);
  pinMode(2, OUTPUT); // pullup // out
  
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(10, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(14, INPUT);
  //pinMode(15, OUTPUT); // pulldown // out
  //pinMode(16, INPUT);

  attachInterrupt(digitalPinToInterrupt(5), handleInterruptA, FALLING);
  attachInterrupt(digitalPinToInterrupt(4), handleInterruptB, FALLING);
}

// the loop function runs over and over again forever
void loop() {
  if (dir == 1) {
    if (val < 1000) {
      val = val + 5;
    } else {
      dir = -1;  
    }
  } else {
    val = val - 5;
    if (val < 1)
      dir = 1;
  }

   
  analogWrite(2, val);

  if (out1)
    digitalWrite(1, LOW);
  else
    digitalWrite(1, HIGH);

  if (out2)
    analogWrite(3, val);
  else
    analogWrite(3, 0);
  
  delay(5);
  /*
  digitalWrite(0, LOW);  
  digitalWrite(1, LOW);  
  digitalWrite(2, LOW);  
  digitalWrite(4, LOW);  
  digitalWrite(5, LOW);  
  digitalWrite(10, LOW);  
  digitalWrite(12, LOW); 
  digitalWrite(13, LOW); 
  digitalWrite(14, LOW); 
  digitalWrite(15, LOW); 
  analogWrite(16, 500); 
  delay(1000);                      // Wait for a second
  digitalWrite(0, HIGH);
  digitalWrite(1, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(15, HIGH);
  analogWrite(16, 250);
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
  */
}

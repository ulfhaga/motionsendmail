#include <stdio.h>
#include <stdio.h>
#include <wiringPi.h>



#define LED1 23                 // wPi:24 (GPIO:19  phypin:35)
#define LED2 24                 // wPi:23 (GPIO:13  phypin:34)

void blinkInit()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}


void led1ON()
{
  digitalWrite(LED1, HIGH);     // On
}

void led1OFF()
{
  digitalWrite(LED1, LOW);      // Off
}


void led1blink()
{
  digitalWrite(LED1, HIGH);     // On
  delay(1000);                  // mS
  digitalWrite(LED1, LOW);      // Off
}

void led1blinking()
{
  for (;;)
  {
    digitalWrite(LED1, HIGH);   // On
    delay(500);                 // mS
    digitalWrite(LED1, LOW);    // Off
    delay(500);
  }
}

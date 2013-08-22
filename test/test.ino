#include "math.h"
#include "Credentials.h"
#include <SPI.h>
#include <WiFly.h>
//#include <avr/wdt.h>

int ledPin = 13;      // select the pin for the LED

void setup()
{
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);

  Serial.println("\n\r\n\rWiFly Shield Terminal Routine");
  
  WiFly.begin();
  
  if (!WiFly.join(ssid))
  {
    Serial.println("Association failed.");
    while (1)
    {
      // Hang on failure.
    }
  }
  
  Serial.print("Associated! IP: ");
  Serial.println(WiFly.ip());
}

void loop()
{      
  pinMode(6, OUTPUT);   // Set digital pin 6 as OUTPUT to power WiFly shield VIN Pin
  digitalWrite(6,HIGH); // trigger relay to power WiFly shield VIN
  
  delay(200); //allow WiFly time to boot
  
  //wdt_enable(WDTO_8S); //start watchdog set for max 8 seconds
  
  /*Serial.print statements used for debugging - can be commented out */
  
  Serial.begin(9600);
  Serial.println("Starting ...");
  //Serial.println(request_url());
  
  delay(100);
  Serial.println("Taking reading...");
  // print the light statusbased on the ldr value: 
  int reading = ldr_reading();
  Serial.println(reading);
  
  //wdt_disable(); //turn off watchdog timer - if sketch gets this far it hasn't hung
  
  //digitalWrite(6,LOW); // Power down WiFly
  
  delay(1000);
}


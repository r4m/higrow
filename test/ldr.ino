#include <String.h>

int sensorPin = A0;    // select the input pin for the light dependent resistor

int sensorValue = 0;  // variable to store the value coming from the sensor

int ldr_reading()                       
{
  // read the analog in value:
  sensorValue = analogRead(sensorPin);   

  return sensorValue;

}

String ldr_msg()
{
  sensorValue = ldr_reading();
  
  // We'll have a few threshholds, qualitatively determined
  if (sensorValue < 10) {
    return " - Dark :(";
  } else if (sensorValue < 200) {
    return " - Dim :/";
  } else if (sensorValue < 500) {
    return " - Light :|";
  } else if (sensorValue < 800) {
    return " - Bright :)";
  } else {
    return " - Very bright ^_^";
  }
}



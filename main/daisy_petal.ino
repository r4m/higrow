// NB: Arduino maps input voltages between 0 and 5 volts into integer values between 0 and 1023. 
// This yields a resolution between readings of: 5 volts / 1024 units or, .0049 volts (4.9 mV) per unit. 

int power = 8;      // power controller (white)

int moisture = A0;  // moisture (red)
int light    = A2;  // light (yellow)
int temp     = A4;  // temperature (blue)

// Daisypetal
const int VCC=5; // V
// GL55 photoresistor
const int R40=10; // kOhm
const float BETA = 1.6021;
const float GAMMA = -0.6021;
// NTCLE100E3 thermistors
const float TA1 = 3.354016E-03;
const float TB1 = 2.569850E-04;
const float TC1 = 2.620131E-06;
const float TD1 = 6.383091E-08;
// Moisture probe
const int R20=100; // kOhm
const int R21=100; // kOhm

int sensors_values_raw[3];
float sensors_values_conv[4];

//int vcc = 5; // power (black)
//int ground = 0; // GND (green)

void initDaisyPetal() {  
  pinMode(power, OUTPUT);
  digitalWrite(power, HIGH);
}

int* readDaisyPetalRAW() {
  //turn on power and wait for stabilization
  digitalWrite(power, LOW);
  delay(500);
  
  sensors_values_raw[0] = analogRead(light);
  //Serial.print("Light = ");
  //Serial.println(sensors_values_raw[0]);
  
  sensors_values_raw[1] = analogRead(moisture);
  //Serial.print("Moisture = ");
  //Serial.println(sensors_values_raw[1]);
  
  sensors_values_raw[2] = analogRead(temp);
  //Serial.print("Temperature = ");
  //Serial.println(sensors_values_raw[2]);
  
  //turn off power for conservation, and to reduce potential thermistor heating, corrosion, and plant electrocution (Davis was empathetic so I added this)
  digitalWrite(power, HIGH);
  
  return sensors_values_raw;
}

float* readDaisyPetalCONV() {
  //turn on power and wait for stabilization

  readDaisyPetalRAW();
  
  sensors_values_conv[0] = R40*(5*sensors_values_raw[0]/(float)1024)/(VCC - (5*sensors_values_raw[0]/(float)1024)); // LDR40 kOhm
//  Serial.print("LDR40 kOhm = ");
//  Serial.print(sensors_values_conv[0]);
  sensors_values_conv[0] = (float)pow(10,((float)log10(sensors_values_conv[0]) - BETA)/GAMMA); // Light Lux
  //Serial.print("Light = ");
  //Serial.print(sensors_values_conv[0]);
  //Serial.println(" [LUX]");
  //Serial.println(lightMsg());
  
  sensors_values_conv[1] = ((R20 + R21)*1000*5*sensors_values_raw[1]/(float)1024 - R20*1000*VCC)/(VCC - (5*sensors_values_raw[1]/(float)1024)); // Moisture Ohm
  sensors_values_conv[1] = sensors_values_conv[1]/1000; // Moisture kOhm
  //Serial.print("Moisture = ");
  //Serial.print(sensors_values_conv[1]);
  //Serial.println(" [kOhm]");
  //Serial.println(moistureMsg());
  
  sensors_values_conv[2] = (5*sensors_values_raw[2]/(float)1024)/(VCC - (5*sensors_values_raw[2]/(float)1024)); // kOhm
  float temperature_kelvin = (float)1/(TA1 + TB1*log(sensors_values_conv[2]) + TC1*pow(2,log(sensors_values_conv[2])) + TD1*pow(3,log(sensors_values_conv[2]))); // Temperature Kelvin
  //Serial.print("Temperature = ");
  //Serial.print(sensors_values_conv[2]);
  //Serial.println(" [K]");
  sensors_values_conv[2] =  temperature_kelvin - 273.15; // Temperature Celsius
  //Serial.print("Temperature = ");
  //Serial.print(sensors_values_conv[2]);
  //Serial.println(" [C]");
  sensors_values_conv[3] =  (temperature_kelvin * 1.8) - 459.67; // Temperature Farenheit
  //Serial.print("Temperature = ");
  //Serial.print(sensors_values_conv[3]);
  //Serial.println(" [F]");  
  
  return sensors_values_conv;
}

String moistureMsg()
{
  long moisture_value = sensors_values_conv[1];
  if (moisture_value <= 10) {
    return " - In water";
  } else if (moisture_value <= 100) {
    return " - Humid soil";
  } else if (moisture_value <= 500) {
    return " - Dry soil";
  } else {
    return " - Floating!?!";
  }
}

String lightMsg()
{
  
  // We'll have a few threshholds, qualitatively determined
  long light_value = sensors_values_conv[0];
  if (light_value <= 0.002) {
    return " - Moonless night :o";
  } else if (light_value <= 1) {
    return " - Full moon on a clear night *_*";
  } else if (light_value <= 80) {
    return " - Hallway :|";
  } else if (light_value <= 100) {
    return " - Very dark day :/";
  }  else if (light_value <= 500) {
    return " - Sunrise :)";
  } else if (light_value <= 1000) {
    return " - Overcast day :)";
  } else {
    return " - Full daylight! %)";
  }
  
  /*
  Illuminance (lux)	Example
  0.002          	Moonless clear night sky
  0.2                   Design minimum for emergency lighting (AS2293).
  0.27 - 1              Full moon on a clear night
  3.4                   Dark limit of civil twilight under a clear sky
  50                    Family living room
  80                    Hallway/toilet
  100                   Very dark overcast day
  300 - 500             Sunrise or sunset on a clear day. Well-lit office area.
  1,000                 Overcast day; typical TV studio lighting
  10,000 - 25,000       Full daylight (not direct sun)
  32,000 - 130,000      Direct sunlight
  */
}


//#include "math.h"
#include <WiServer.h>
#include "credentials.h"
#include <avr/io.h>
#include <avr/wdt.h>

//#define Reset_AVR() wdt_enable(WDTO_30MS); while(1) {}

//int led = 13;
//byte ledState = 0; 

unsigned long updateTime = 0; // [ms] time to retrieve data from sensors
//unsigned long watchdog = 0;
int sensorsData = 4;

uint8 ip[] = {173,203,98,29};
char serverName[] = "api.pachube.com\nX-PachubeApiKey: INSERT-HERE-YOUR-COSM-API-KEY\nConnection: close";
char url[] = "/v1/feeds/58525.csv?_method=put";
//char serverName[] = "www.pachube.com\nX-PachubeApiKey: INSERT-HERE-YOUR-COSM-API-KEY\nConnection: close";
//char url[] = "/api/53765.csv?_method=put";
int portNumber = 80;

POSTrequest postPachube(ip, portNumber, serverName, url, feedData);

float *sensors_values;
String PachubeOutputCSV;
    
void setup()
{
  //pinMode(led, OUTPUT);
  //digitalWrite(led, HIGH);
  
  Serial.begin(9600); 

  WiServer.enableVerboseMode(true);

  Serial.print("\nActivating WiFi shield...");
  //WiServer.init(webpage_request);
  WiServer.init(NULL);
  Serial.println("done.");
  initDaisyPetal();
  //postPachube.setReturnFunc(printData); 

  /* DO NOT WORK :/
   if(WiServer.isConnected())
   Serial.println("WiFi associated!");
   else
   Serial.println("Unable to estabilish connection.");
   */
   
   wdt_enable(WDTO_8S);
}

void loop()
{ 
  /*if(watchdog > 40000)
  {
    Serial.print("Watchdog expired. Resetting.");
    Reset_AVR();
  }*/
  wdt_reset();
  
  if (millis() >= updateTime) {
        
    Serial.print("Time: ");
    Serial.println(updateTime);
    updateTime += (long)1000 * 120;  // Get another update three minutes from now
    
    Serial.print("Reading data...");
    sensors_values = readDaisyPetalCONV();
    Serial.println("done.");
    
    /*char lightString[5]; 
    memcpy(lightString, &sensors_values[0], sizeof (sensors_values[0]));
    char tempString[5]; 
    memcpy(tempString, &sensors_values[1], sizeof (sensors_values[1]));
    char humiString[5]; 
    memcpy(humiString, &sensors_values[2], sizeof (sensors_values[2]));    
    char* allCSVdata =  (char*)malloc(strlen(lightString)+1+strlen(tempString)+1+strlen(humiString));
    strcpy(allCSVdata, lightString); 
    strcat(allCSVdata, ","); 
    strcat(allCSVdata, tempString); 
    strcat(allCSVdata, ","); 
    strcat(allCSVdata, humiString); 
    free(allCSVdata);*/
    
    PachubeOutputCSV = "";
    float value;
    String valueAsString;  
    for(int i=0; i<sensorsData; i++)
    {
      char dataBuffer[10];
      value = sensors_values[i];
      dtostrf(value,1,2,dataBuffer);
      valueAsString = String(dataBuffer);        
      PachubeOutputCSV += valueAsString;
      if(i<(sensorsData-1))
      { 
        PachubeOutputCSV += ",";
      }
    }

    //Serial.println(PachubeOutputCSV); // TODO: capire perche' non riesce a visualizzare...
    Serial.print("Submitting to Pachube...");
    postPachube.submit();
    Serial.println("done.");
    
    //watchdog = 0;  
  }

  //watchdog++;
  WiServer.server_task();
  delay(10);
}

void feedData(){   
    WiServer.print(PachubeOutputCSV);
}


// Function that prints data from the server
/*void printData(char* data, int len) {
 
 // Print the data returned by the server
 // Note that the data is not null-terminated, may be broken up into smaller packets, and 
 // includes the HTTP header. 
 while (len-- > 0) {
 Serial.print(*(data++));
 } 
 }*/


boolean webpage_request(char* URL){
  if (strcmp (URL, "/") == 0)
  {
    send_webpage();
    return true;
  }
  return false;
}

void send_webpage(){
  WiServer.print ("<html>");
  WiServer.print ("<head>");
  WiServer.print ("<meta http-equiv=""refresh"" content=""10;url=http://");
  WiServer.print (local_ip[0], DEC);
  WiServer.print (".");
  WiServer.print (local_ip[1], DEC);
  WiServer.print (".");
  WiServer.print (local_ip[2], DEC);
  WiServer.print (".");
  WiServer.print (local_ip[3], DEC);
  WiServer.print ("/"" />");
  WiServer.print ("</head>");

  WiServer.print ("<p align=""center"">");
  WiServer.print ("Hello World!<br>");
  WiServer.print ("I'm definitely in! o_o <br>");
  WiServer.println("<br>");
  WiServer.println("<br>");
  WiServer.print("<strong>Temperature: </strong>");
  WiServer.print(sensors_values[2]);
  WiServer.print(" Celsius");
  WiServer.println("<br>");
  WiServer.print("<strong>moisture: </strong>");
  WiServer.print(sensors_values[1]);
  WiServer.print("%");
  WiServer.println("<br>");
  WiServer.print("<strong>Light: </strong>");
  WiServer.print(lightMsg());
  WiServer.println("<br>");

  WiServer.print ("</html>");
}



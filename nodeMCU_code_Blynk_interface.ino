/*
 * blynk template_ID, template-name, authentication token shoud be filled up before using the this code 
 * WiFi credentials should also be filled up
 * ALl the dependecies should be addressed before using this code
 * to read data from arduino serial communication is used
 * Serial.parseFloat is used to read data from arduino
 * as i was not able to read sensor data from DHT22 on esp8266
 */

#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>

//#include <DHT.h>


 

// You should get Auth Token in the Blynk App.

// Go to the Project Settings (nut icon).


 

// Your WiFi credentials.

// Set password to "" for open networks.

char ssid[] = "";

char pass[] = "";

BlynkTimer timer;


// This function sends Arduino's up time every second to Virtual Pin (5).

// In the app, Widget's reading frequency should be set to PUSH. This means

// that you define how often to send data to Blynk App.

void new_map()

{
  float myFloat = 0;
  // You can send any value at any time.
  //float t = (((analogRead(temp) - 0)*(125-(-40)))/(1023))+(-40); //new_val = ((old_val - old_lower_lim)*(new_range))/(old_range) + new_lower_lim
  // Please don't send more that 10 values per second.
  if (Serial.available() > 0) {
    myFloat = Serial.parseFloat();

    // prints the received float number
    Serial.print("I received: ");
    Serial.println(myFloat);
  //Serial.println(t);
  }
  Blynk.virtualWrite(V6, myFloat);
 /*
    This pins are from the same pins that are connected to arduino-3 to issue the warnings
  */
  Blynk.virtualWrite(V1, digitalRead(D1));
  Blynk.virtualWrite(V0, digitalRead(D0));
  Blynk.virtualWrite(V2, digitalRead(D2));
  delay(2000);

}

 

void setup()

{

  // Debug console

  Serial.begin(9600);
  Serial.begin(115200);
 
  pinMode(D1, INPUT);
  pinMode(D0, INPUT);
  pinMode(D2, INPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, new_map);

}

 

void loop()

{

  Blynk.run();

  timer.run();

}

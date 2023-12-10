/*
 * this code is uploaded to the second arduino which controlls the security part of this project
 * This code utilizes 4X3 keypad and SPDT momentary action switch
 * When system is armed and switch is open meaning INPUT is low to arduino warning is issued
 * For warning pupose arduino sends high signal to arduino 1 which in turn sends a signal to warning arduino or arduino-3
 * to issue warning
 */

#include <Keypad.h>  /*Included Keypad library*/
#include <string.h>
const byte ROWS = 4; /*Define Keypad Rows*/
const byte COLS = 3; /*Define Keypad Columns*/
char Keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'},
};
int green_led = 11;
int red_led = A3;
int buzzer = A2;
byte rowPins[ROWS] = {2,3,4,5};  /*Initialized Arduino Pins for Rows*/
byte colPins[COLS] = {6,7,8};     /*Initialized Arduino Pins for Columns*/
Keypad myKeypad = Keypad(makeKeymap(Keys), rowPins, colPins, ROWS, COLS); /*Function for Keypad*/
char pass_in[4]; //input string variable
int Y = 0;       
int count = 0;  //changes array position to next position to store next character
int intrusion_count = 0; //counts passcode input attempt
char pass[6] = "1234";
int door = 12;
int warning1 = A4;
int check = 0;
int warning2 = 10;
void setup(){
  pinMode(green_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(door, INPUT);
  pinMode(warning1, OUTPUT);
  pinMode(warning2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);/*Baud Rate for Serial Communication*/
}
void loop(){
  /*Take input from Keypad*/
  char Key = myKeypad.getKey();
  if(Key == '*'){ // Arms the Security System
       digitalWrite(green_led, LOW);
       Y = 1;
      }
   else{
      while (Y == 1){
        doors();
        digitalWrite(red_led, HIGH);
        char Key = myKeypad.getKey();
        if (intrusion_count < 3){
          digitalWrite(red_led, HIGH);
        }
        if (Key != '#'){ //FInishes Taking input when # is pressed
          if (Key){
            pass_in[count] = Key;
            Serial.println(Key);
            count++;
            digitalWrite(buzzer, HIGH);
            delay(50);
            digitalWrite(buzzer, LOW);
          }
        }
        else{ //Adds '\0' to signify string end after passcode input ends
          pass_in[count] = '\0';
          Serial.println(pass_in);
          Serial.println(pass);
          count = 0;
          Y = 0;
          check = 1;
          }
      }
   }
   if(check == 1){ // CHecks Whether Passcode is correct of not 
      if(strcmp(pass, pass_in) == 0){ // Correct...system disarm
          digitalWrite(green_led, HIGH);
          digitalWrite(red_led, LOW);
          digitalWrite(warning1, LOW);
          intrusion_count = 0;
          Serial.println(strcmp(pass, pass_in));
          check = 0;
          digitalWrite(warning2, LOW);
        }
      else{ // incorrect but less than threshold
        intrusion_count++;
        digitalWrite(green_led, LOW);
        if (intrusion_count < 3){
          digitalWrite(warning2, LOW);
        }
        else{ // more than threshold...start warning
          digitalWrite(warning2, HIGH);
          }
        check = 0;       
        }
   }
}



void doors()
{
  if(digitalRead(door) == LOW){ // System armed and door closed
      digitalWrite(warning1, HIGH);
      Serial.println("Door Open");
    }
    else if(digitalRead(door) == HIGH){ // System armed and door open...warning
      digitalWrite(warning1, LOW);
      Serial.println("Door Closed");
      }  
}

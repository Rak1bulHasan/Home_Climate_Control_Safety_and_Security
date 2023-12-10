#include <dht.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27,16,2);
#define dataPin 4 // Defines pin number to which the sensor is connected
dht DHT; // Creats a DHT object
SoftwareSerial espSerial(5, 6);
int set = A0;
int up = A1;
int down = A2;
int back = A3;
float preset_temp = 25;
int Y = 0;
int upper_hum = 60;
int lower_hum = 40;
int AC_ON = 10;
int gas_sensor = A6;
int warning_LED = 7;
int motor_in1 = 8;
int motor_in2 = 9;
int motor_enA = 3;
int window = 11;
int door = 12;
int warning = 13;
void setup()
{
  pinMode(set, INPUT);
  pinMode(up, INPUT);
  pinMode(down, INPUT);
  pinMode(back, INPUT);
  pinMode(AC_ON, OUTPUT);
  pinMode(gas_sensor, INPUT);
  pinMode(warning_LED, OUTPUT);
  pinMode(motor_in1, OUTPUT);
  pinMode(motor_in2, OUTPUT);
  pinMode(motor_enA, OUTPUT);
  pinMode(window, INPUT);
  pinMode(warning, OUTPUT);
  pinMode(door, INPUT);
  Serial.begin(9600);
  espSerial.begin(115200);
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
}
void loop() 
{
  //Uncomment whatever type you're using!
  int readData = DHT.read22(dataPin); // DHT22/AM2302
  float t = DHT.temperature; // Gets the values of the temperature
  float h = DHT.humidity; // Gets the values of the humidity
  espSerial.println(t);
  while (Y == 1){
      if (digitalRead(up) == HIGH){
        preset_temp = preset_temp + 0.1;
        temp_set(preset_temp);
        }
      else if(digitalRead(down) == HIGH){
        preset_temp = preset_temp - 0.1;
        temp_set(preset_temp);
        }
      else if(digitalRead(back) == HIGH){
          Y = 0;
        }
       else{
        temp_set(preset_temp);
        }
      }
  
  if (digitalRead(set) == HIGH){
    Y = 1;
    }
  else if(preset_temp >= t){
    Serial.println("Temp is lower than Preset.");
    if(lower_hum <= h && h <= upper_hum){
         digitalWrite(AC_ON, LOW);
         Serial.println("Humidity is Optimum");
            }
    else{
         digitalWrite(AC_ON, HIGH);
         Serial.println("HUmidity is not Optimum.");
      }
      }
  else{
    Serial.println("Temp is Higher than Preset.");
          if(lower_hum <= h && h <= upper_hum){
            Serial.println("Humidity is Optimum");
            }
          digitalWrite(AC_ON, HIGH);
      }
  gas_check();
  data_print(t,h);
  warning_issue();

  delay(2000); // Delays 2 secods
}

void data_print(float t, float h)
{
  // Print a message on both lines of the LCD.
  lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
  lcd.print("TEMP(C)   HUM(%)");
  lcd.setCursor(0,1);
  lcd.print(String(t)+' '+' '+' '+' '+' '+' ' +String(h));
}

void temp_set(float n_t)
{
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("Set Temp: "+String(n_t));
  lcd.setCursor(0,1);
  lcd.print("                ");
}

void gas_check()
{
  Serial.println(analogRead(gas_sensor));
  if(analogRead(gas_sensor) > 500){
    Serial.println("Hello");
    digitalWrite(warning_LED, HIGH);
    delay(200);
    digitalWrite(warning_LED, LOW);
    delay(200);
    windows();
    }
    else {
      digitalWrite(warning_LED, LOW);
      }
}

void windows()
{
  if (digitalRead(window) == LOW){
      digitalWrite(motor_in1, HIGH);
      digitalWrite(motor_in2, LOW);
      analogWrite(motor_enA, 60);
    }
    else{
      digitalWrite(motor_in1, LOW);
      digitalWrite(motor_in2, LOW);
      }
}

void warning_issue()
{
  if (digitalRead(door) == HIGH){
      digitalWrite(warning, HIGH);
    }
  else{
    digitalWrite(warning, LOW);
    }
}

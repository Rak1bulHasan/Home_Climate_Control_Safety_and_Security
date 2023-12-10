/*
 * This code is uploaded to Arduino-3. 
 * this code utilizes the buzzer and LED connected to it to issue various warnings
 * according to the input received from arduino-1 and arduino-2
 * Priority Scheme: Hazard > Intrusion > Multiple Wrong Passcode
 */

int RECV_PIN = 11;
int LED = 2;
int Buzzer = 3;
int warning_pass = 10; //Warning when wrong passcode threshold is crossed
int warning_door = 9;  //Warning when intrusion is detected while the system is armed 
int hazard = 8;        //Warning when Hazard is detected

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(warning_pass, INPUT);
  pinMode(warning_door, INPUT);
  pinMode(hazard, INPUT);
  Serial.begin(9600);
}

void loop() {
  delay(100);
  Serial.println(digitalRead(hazard));
  if(digitalRead(hazard) == LOW){ // Hazard has to low
    security();
  }
  else{ //Hazard
    Hazard();
    }
}

void security()
{
  if(digitalRead(warning_door) == HIGH){ //intrusion
    digitalWrite(LED, HIGH);
    digitalWrite(Buzzer, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    digitalWrite(Buzzer, LOW);
    Serial.println("HEllo");
    }
    else if(digitalRead(warning_door) == LOW && digitalRead(warning_pass) == HIGH){
      digitalWrite(Buzzer, HIGH);
      Serial.println("HEY");
      delay(400);
    }
    else{
      digitalWrite(LED, LOW);
      digitalWrite(Buzzer, LOW);
      Serial.println("Hi");
      }
}

void Hazard()
{
  //Hazard detected
   digitalWrite(LED, HIGH);
   digitalWrite(Buzzer, HIGH);
   delay(800);
   digitalWrite(LED, LOW);
   digitalWrite(Buzzer, LOW);
   Serial.println("Y0");
}

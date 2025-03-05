#include <SoftwareSerial.h>
#include<string.h>
#include <LiquidCrystal.h>
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);   //initialise LCD display

int pump=13;   //pin for contrlling water pump
int LED=12;    //pin for controlling LEDs
int buz=11;    //pin for controlling buzzer
int d=0;

int ledState = LOW;    //initial LED state
int pumpState = LOW;   //initial water pump state
unsigned long previousMillis = 0; 
const long interval = 200;  
unsigned long previousMillis2 = 0; 
const long interval2 = 30000;  

void setup() {
   
    lcd.begin(16, 2);          //start communication with LCD
    pinMode(LED, OUTPUT);      //initialise LED pin as OUTPUT
    pinMode(buz, OUTPUT);      //initialise buzzer pin as OUTPUT
    pinMode(pump, OUTPUT);     //initialise pump pin as OUTPUT
    lcd.print("HEllO");
    Serial.begin(9600);        //start serial communication with PC
}
long time;
void loop() {
   
   if (Serial.available() > 0) {
     d=Serial.read();        //read output from PC
    if(d == 'p') {         //if fire detected
      if(millis() - time > 500){
        lcd.clear();
        lcd.print(" Fire Detected ");    //diaplay message on LCD
        Serial.println("PHAT HIEN CHAY TREN DAY DIEN");
        Blink();           //activate water pump, LEDs and buzzer
      
    }
    }
    if(d == 's') {         //if no fire detected
      time = millis();
        lcd.clear();
        lcd.print("Capturing Video..."); 
        Serial.println("KHONG PHAT HIEN"); 
        digitalWrite(pump, LOW);  
        digitalWrite(LED, LOW); 
        digitalWrite(buz, LOW); 
       
    }
}
}


void Blink() {      //Function for controlling LEDs, pump, buzzer
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        ledState = HIGH;
        digitalWrite(LED, HIGH);
        digitalWrite(buz, HIGH);
        digitalWrite(pump, HIGH);
        delay(5000);
        
    }
}

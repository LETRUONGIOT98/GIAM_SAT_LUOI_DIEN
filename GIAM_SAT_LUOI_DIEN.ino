#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
const char* ssid = "COFFEE BEN PS4";   // Tên wifi 
const char* password = "137hoangdieu2";   // mật khẩu wifi
WiFiClient  client;
const char * myWriteAPIKey = "6ZVMJNTJWJH0TKMJ";

#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>

#if defined(ESP32)
    #error "Software Serial is not supported on the ESP32"
#endif

/* Use software serial for the PZEM
 * Pin 12 Rx (Connects to the Tx pin on the PZEM)
 * Pin 13 Tx (Connects to the Rx pin on the PZEM)
*/
#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
#define PZEM_RX_PIN D7
#define PZEM_TX_PIN D6
#endif


SoftwareSerial pzemSWSerial(PZEM_RX_PIN, PZEM_TX_PIN);
PZEM004Tv30 pzem(pzemSWSerial);
#include <LiquidCrystal_I2C.h>    //Thư viện màn hình LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);  //Khai báo địa chỉ và kích thước màn hình
char d=' ';
long times,time1,time2;
int tt =0;
float voltage = 0;
    float current = 0;
    float power =0;
    float energy = 0;
    float frequency = 0;
    float pf=0;

void setup() {
    /* Debugging serial */
    Serial.begin(115200);
  lcd.init();                    //Khởiu động màn hình
  lcd.backlight();   //Bật đèn màn hình 
  lcd.setCursor(0, 0);   //
  lcd.print(" DANG KET NOI WIFI  ");
  WiFi.mode(WIFI_STA);   
  WiFi.begin(ssid, password); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  Serial.println("DANG KET NOI WIFI");
      while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(500);     
      }
  times = millis();
  time1 = millis();
lcd.setCursor(0, 3);   //
  lcd.print("     BINH THUONG    "); 
  ThingSpeak.setField(7, 0);
 delay(1000);
}

void loop() {
    if (Serial.available() > 0) {
     d=Serial.read();        //read output from PC
    if(d == 'p') {         //if fire detected
      if(millis() - times > 100){
        time2 = millis();
        lcd.setCursor(0, 3);   //
  lcd.print("DUONG DAY PHONG DIEN"); 
  tt=1;
  }
    }
    if(d == 's') {         //if no fire detected
      times = millis();
      if(millis() - time2 > 20000){
      lcd.setCursor(0, 3);   //
  lcd.print("     BINH THUONG    "); 
   tt=0;
      }
    }
    }
    ThingSpeak.setField(7, tt);
    Serial.print("Custom Address:");
    Serial.println(pzem.readAddress(), HEX);

    // Read the data from the sensor
    
     voltage = pzem.voltage();
     current = pzem.current();
     power = pzem.power();
     energy = pzem.energy();
     frequency = pzem.frequency();
    float pf = pzem.pf();
    // Check if the data is valid
    if(isnan(voltage)){
        Serial.println("Error reading voltage");
        voltage = 0;
    } else if (isnan(current)) {
        Serial.println("Error reading current");
    } else if (isnan(power)) {
        Serial.println("Error reading power");
    } else if (isnan(energy)) {
        Serial.println("Error reading energy");
    } else if (isnan(frequency)) {
        Serial.println("Error reading frequency");
    } else if (isnan(pf)) {
        Serial.println("Error reading power factor");
    } else {
        
        // Print the values to the Serial console
        Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
        Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
        Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
        Serial.print("Energy: ");       Serial.print(energy,3);     Serial.println("kWh");
        Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
        Serial.print("PF: ");           Serial.println(pf);
        
    }
    lcd.setCursor(0, 0);   //
  lcd.print("U:      V  I:      A");
  lcd.setCursor(3, 0);
  lcd.print(voltage,1);  //In giá trị vol lên lcd
  lcd.setCursor(14, 0);
  lcd.print(current,2);  // in giá rtrị dòng điện lên lcd

  lcd.setCursor(0, 1);
  lcd.print("P:        W F:    Hz");
  lcd.setCursor(3, 1);
  lcd.print(power, 1);  // hiển thị giá trị công suất lên dòng điện 
  lcd.setCursor(14, 1);
  lcd.print(frequency, 1);//hiển thị nhiệt độ lên lcd

  lcd.setCursor(0, 2);
  lcd.print("E:       KWh pF:    ");
  lcd.setCursor(2, 2);
  lcd.print(energy,3);
  lcd.setCursor(16, 2);
  lcd.print(pf, 2);
    Serial.println();
  if(millis() - time1 > 15000){
    ThingSpeak.setField(1, voltage);
        ThingSpeak.setField(2, current);
        ThingSpeak.setField(3, power);
        ThingSpeak.setField(4, energy);
        ThingSpeak.setField(5, pf);
        ThingSpeak.setField(6, frequency);
        
        int x = ThingSpeak.writeFields(1, myWriteAPIKey);

    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    time1 = millis();
  }
}

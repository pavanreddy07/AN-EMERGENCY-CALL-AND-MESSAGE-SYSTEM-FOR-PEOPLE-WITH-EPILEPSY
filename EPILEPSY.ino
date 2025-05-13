#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
SoftwareSerial uart(8, 9);
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#include <TinyGPS.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#define button A0
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
TinyGPS gps;
float flat=0, flon=0;
int buz=10;
void read_gps()
{
    bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      if (gps.encode(c)) 
        newData = true;
    }
  }
  if (newData)
  {
    
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);

  }
}
int cnt=0;
void setup()
{   lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Emergency call ");
  lcd.setCursor(0, 1);
  lcd.print("Message system");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("for people with");
  lcd.setCursor(0, 1);
  lcd.print("epilepsy");
  delay(1000);
   Serial.println("AT");
  delay(1500);
  Serial.println("AT+CMGF=1");
  delay(500);
    lcd.clear();
  Serial.begin(9600);
    uart.begin(9600);
  accel.begin();
  pinMode(buz,OUTPUT);
  pinMode(button,INPUT);
  digitalWrite(buz,0);
  delay(2000);

}

void loop()
{   delay(200);
    sensors_event_t event; 
    accel.getEvent(&event);
    int xval=event.acceleration.x;
    int yval=event.acceleration.y;
    int bval = analogRead(button);
    lcd.clear();
     lcd.setCursor(0,0);
    lcd.print("X:"+ String(xval)+"  Y:"+ String(yval) );
    lcd.setCursor(0,1);
    lcd.print("V:"+ String(bval) );
    cnt++;
if(cnt>4){
 uart.print("2850274,RJOUAMHNC2M35RE6,0,0,project1,12345678,"+String(xval)+","+String(yval)+","+String(bval)+","+String(16.3510)+","+String(81.0426)+",\n");
 cnt=0;
  }
if((bval>600))
    {
      digitalWrite(buz,1);
          send_sms(1);
          delay(4000);
      makeCall();
  
      digitalWrite(buz,0);   
    }   
    if((xval<-5)|| (xval>5) || (yval>5) || (yval<-5))
    { lcd.clear();
      lcd.setCursor(0,0);
    lcd.print(" FALSE   ");   
    send_sms(2);
          
    }
else
 {
   digitalWrite(buz,0);
  }
}
void makeCall() {
  Serial.println("AT"); 
  delay(1000);
  Serial.println("ATD+917013451264;"); // Dial the number
  lcd.setCursor(0, 1);
  lcd.print("Call going on...");
  delay(20000); 
  Serial.println("ATH");
  delay(1000);
}

void send_sms(int k)
  {    
Serial.println("Sending SMS...");
Serial.println("AT");    
delay(1000);  
Serial.println("ATE0");    
delay(1000);  
Serial.println("AT+CMGF=1");    
delay(1000);  
Serial.print("AT+CMGS=\"7013451264\"\r\n");
delay(1000);
if(k==1){
Serial.println(" EPILEPSY DETECTED: ");
Serial.println("https://www.google.com/maps/search/?api=1&query=" + String(16.3510)+ "," + String(81.0426));
lcd.clear();
lcd.setCursor(0,0);
lcd.print("message sent "); 
}
if(k==2)
Serial.println(" FALSE DETECTION: ");
delay(500);
Serial.print(char(26));
delay(2000); 
  }

//format  "WRITE W_CH_ID,W_API,R_CH_ID,R_API,SSID,PSW,f1,f2,f3,f4,f5,f6,f7,f8\n"
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
int statusCode = 0;
unsigned long lastTime = 0;
unsigned long timerDelay = 16000;
WiFiClient  client;
const int FieldNumber1=1;
String strs[14]={"2838060","P6Z07T35NVBAGTEV","0","0","project1","12345678","0","0","0","0","0","0","0","0"};
int StringCount =0;
int prv=0;
int led=D4;
void setup()
{
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  Serial.begin(9600);
  pinMode(led,OUTPUT); 
  delay(1000);
  digitalWrite(led,1);
}

void loop()
{
  //----------------- Network -----------------//
  if (WiFi.status() != WL_CONNECTED)
  {
    //Serial.print(".");
    delay(1000);
    WiFi.begin(strs[4], strs[5]);
      for(int kk=0;kk<10;kk++)
      {
        digitalWrite(led,0);
        delay(300);
        digitalWrite(led,1);
        delay(300);
      }
   if(WiFi.status() == WL_CONNECTED)
    Serial.println("ok");
  }
 
  //---------------- Channel 1 ----------------//
  const char* string2 = strs[2].c_str();
  const char* string3 = strs[3].c_str();
  int temp = ThingSpeak.readLongField(atol(string2), FieldNumber1, string3);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    if(temp !=prv)
    {
      prv=temp;
       Serial.print(temp);
    }
   
    
  }
  
  delay(100);
  

if (Serial.available()) 
{
String rcv = Serial.readStringUntil('\n');

if ((millis() - lastTime) > timerDelay) 
{

   StringCount=0;
     while (rcv.length() > 0)
  {
    int index = rcv.indexOf(',');
    if (index == -1) // No space found
    {
      strs[StringCount++] = rcv;
      break;
    }
    else
    {
      strs[StringCount++] = rcv.substring(0, index);
      rcv = rcv.substring(index+1);
    }
  }
    
 ThingSpeak.setField(1, strs[6]);
 ThingSpeak.setField(2, strs[7]);
 ThingSpeak.setField(3, strs[8]);
 ThingSpeak.setField(4, strs[9]);
 ThingSpeak.setField(5, strs[10]);
 ThingSpeak.setField(6, strs[11]);
 ThingSpeak.setField(7, strs[12]);
 ThingSpeak.setField(8, strs[13]);
 const char* string0 = strs[0].c_str();
 const char* string1 = strs[1].c_str();
 int x = ThingSpeak.writeFields(atol(string0), string1);
 if(x == 200){
  delay(10);
  }
else{
  delay(10);
  }
  lastTime = millis();
 }
 
}
  delay(500);
}

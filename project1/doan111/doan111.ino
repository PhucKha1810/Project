#include "DHT.h"  
#include <AccelStepper.h>
#include <Wire.h>
#include <ds3231.h>
#define motorPin1  8     
#define motorPin2  9      
#define motorPin3  10    
#define motorPin4  11  
const int DHTPIN = 7;      
const int DHTTYPE = DHT11;     
int relaydat = 2;    
int cbdat = A0;     
int doc_cb, TBcb;
int relaykk = 3;
DHT dht(DHTPIN, DHTTYPE);
#define MotorInterfaceType 8
int sang = 0;
AccelStepper stepper = AccelStepper(MotorInterfaceType, motorPin1, motorPin3, motorPin2, motorPin4);
int relayds3231 = 4; 
struct ts t;
void setup()
  {
    Serial. begin (9600);        
    pinMode (cbdat, INPUT);      
    pinMode (relaydat, OUTPUT);   
    digitalWrite (relaydat, LOW);
    dht.begin(); 
    pinMode(A1, INPUT);
    stepper.setMaxSpeed(1000);
    pinMode (relayds3231, OUTPUT);
    Wire.begin();
    DS3231_init(DS3231_CONTROL_INTCN);
    t.hour=12; 
    t.min=30;
    t.sec=0;
    t.mday=25;
    t.mon=12;
    t.year=2019; 
    DS3231_set(t);  
  }

void loop() 
  {      
    for(int i=0;i<=9;i++)                              
        {
          doc_cb += analogRead(cbdat);     
        }
    TBcb=doc_cb/10;       
    int phantramao = map(TBcb, 0, 1023, 0, 100);    
    int phantramthuc = 100 - phantramao;        
    doc_cb=0;   
    if ( phantramthuc >= 30 )      
      {
        digitalWrite (relaydat, LOW);   
      }    
    else                            
      {
        digitalWrite (relaydat, HIGH);   
      }
  int h = dht.readHumidity();    
  int t1 = dht.readTemperature(); 
  if ( t1 < 40 )       
      {
        digitalWrite (relaykk, LOW);    
      }
    
   else                             
      {
        digitalWrite (relaykk, HIGH);   
      }  
  unsigned int value = analogRead(A1); 
  if (sang == 0){
    if ( value < 15 ){
      sang = 1;
      Serial.println(sang);
      stepper.setCurrentPosition(0);  
      while (stepper.currentPosition() != 4096) {
        stepper.setSpeed(1000);
        stepper.runSpeed();       }   
      }
   }
  else if (sang==1) {
    if (value > 50){
    sang = 0;
    Serial.println(sang);
    stepper.setCurrentPosition(0);
    while (stepper.currentPosition() != -4096) {
      stepper.setSpeed(-1000);
      stepper.runSpeed();
    }  
  }} 

  DS3231_get(&t);
  if ( (t.min % 1 ==0) && (t.sec < 15))         
      { digitalWrite (relayds3231, HIGH);  
        Serial.println("ON"); 
      }    
  else                             
      { digitalWrite (relayds3231, LOW);
        Serial.println("OFF"); 
      }
   Serial.println(phantramthuc); 
   Serial.println(t1);
   Serial.println(value);
    if ( phantramthuc >= 30 )               
      {
        Serial.println("OFF");         
      }    
    else                            
      { 
        Serial.println("ON"); 
      }
   if ( t1 < 40 )              
      { Serial.println("OFF");        
      }    
   else                         
      { 
        Serial.println("ON");       
      }      
   if (sang == 0){    
      Serial.println("OFF");
      }
   else if (sang==1) {  
    Serial.println("ON");    
  }   
   delay(500);
  }

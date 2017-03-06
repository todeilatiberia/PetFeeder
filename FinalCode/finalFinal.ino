#include <SD.h>
#include <Wire.h>
#include <Time.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Servo.h>
#include <EEPROM.h>     
#include <SPI.h>        
#include <MFRC522.h>

String voice; 

#define SS_PIN 10
#define RST_PIN 9
 
Servo myservo;

boolean match = false;        
boolean programMode = false;  
boolean replaceMaster = false;

int lightSensor = 0;
int distanceSensor=1;
int pos = 0; 
int successRead; 

byte storedCard[4];  
byte readCard[4];   
byte masterCard[4];


MFRC522 mfrc522(SS_PIN, RST_PIN); 

void setup() {
  pinMode(8, OUTPUT); 
  setSyncProvider(RTC.get); 
  myservo.attach(9);
  Serial.begin(9600);  
  SPI.begin();           
  mfrc522.PCD_Init();    
  if (EEPROM.read(1) != 143) {
    do 
    {
      successRead = getID();            
    }
    while (!successRead);                  
    for ( int j = 0; j < 4; j++ ) 
    {        
      EEPROM.write( 2 + j, readCard[j] );  
    }
    EEPROM.write(1, 143);                 
    
  }
 for ( int i = 0; i < 4; i++ ) 
 {          
    masterCard[i] = EEPROM.read(2 + i);    
    Serial.print(masterCard[i], HEX);
    Serial.println("");
  }
  
}



void loop() 
{
  int valueFromLightSensor = analogRead(lightSensor);
  //Serial.print("Light Value= ");
  //Serial.println(valueFromLightSensor);
  //Serial.println("");
  //Serial.print("Distance Value= ");
  
  int valueFromDistanceSensor = analogRead(distanceSensor);
  int distance= 4800/(valueFromDistanceSensor - 20);
  //Serial.println(distance);
  
  //Serial.print("Hour= ");
 // Serial.println(hour());
 
 
  while (Serial.available())
  { 
  delay(10); 
  char c = Serial.read(); 
  voice += c; 
  }  
  if (voice.length() > 0)
  {
    Serial.println(voice); 
       if(voice == "feed")
       {
         myservo.write(130);  
         delay(1000);             
         myservo.write(50);  
         delay(1000);                 
         myservo.write(130); 
         delay(1000);  
         myservo.write(50);
         delay(1000);  
         digitalClockDisplay();
        } 
       if(voice == "feed2")
       {
         myservo.write(130);  
         delay(1000);             
         myservo.write(50);  
         delay(1000);                 
         digitalClockDisplay();
        } 
        if(voice == "feed1")
       {
         myservo.write(130);  
         delay(1000);             
         myservo.write(50);  
         delay(1000); 
         myservo.write(130);  
         delay(1000);             
         myservo.write(50);  
         delay(1000);     
         myservo.write(130);  
         delay(1000);             
         myservo.write(50);  
         delay(1000);      
         digitalClockDisplay();
        } 
 voice="";
 }

  
 do {
    successRead = getID();  
   
  }
  while (!successRead);   
  if (programMode) {
    if ( isMaster(readCard) ) { 
       programMode = false;
      return;
    }
    else {
      if ( findID(readCard) ) {
        
      }
      
    }
  }
  else {
    if ( isMaster(readCard)) {    
      programMode = true;
           int count = EEPROM.read(0);   
     
     
    }
    else {
      if ( findID(readCard) ) { 
       
                if ((hour()>=8) && (hour()<=12 )){
                  if (distance>=20){
                   //   Serial.println(distance);
                      myservo.write(130);  
                      delay(100);             
                      myservo.write(50);  
                      delay(100);                 
                      myservo.write(130); 
                      delay(100);  
                      myservo.write(50);
                      delay(100);  
                      digitalClockDisplay();
                      
                  }
                delay(300);  
                }
                
                
       if ((hour()>=12) && (hour()<=16 )){
                  if (distance>=20){
                    //  Serial.println(distance);
                      myservo.write(130);  
                      delay(100);             
                      myservo.write(50);  
                      delay(100);                 
                      myservo.write(130); 
                      delay(100);  
                      myservo.write(50);
                      delay(100);  
                      digitalClockDisplay();
                      
                  }
                delay(300);  
          }
          
          
         if ((hour()>=16) && (hour()<=20 )){
                  if (distance>=20){
                    //  Serial.println(distance);
                      myservo.write(130);  
                      delay(100);             
                      myservo.write(50);  
                      delay(100);                 
                      myservo.write(130); 
                      delay(100);  
                      myservo.write(50);
                      delay(100);  
                      digitalClockDisplay();
                  }
                delay(300);  
          }
                
           if ((hour()>=20) && (hour()<=8 )){
                  if (distance>=20){
                     // Serial.println(distance);
                      myservo.write(130);  
                      delay(100);             
                      myservo.write(50);  
                      delay(100);                 
                      myservo.write(130); 
                      delay(100);  
                      myservo.write(50);
                      delay(100);  
                      digitalClockDisplay();
                      
                  }
                delay(300);  
          }      
                
                
      }
      else {      // If not, show that the ID was not valid
        Serial.println(F("You shall not pass"));
       
      }
    }
  }
}

int getID() {

  if ( ! mfrc522.PICC_IsNewCardPresent()) { 
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   
    return 0;
  }
 // Serial.println(F("Scanned PICC's UID:"));
  for (int i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
 //   Serial.print(readCard[i], HEX);
  }
//  Serial.println("");
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}

void readID( int number ) {
  int start = (number * 4 ) + 2;    
  for ( int i = 0; i < 4; i++ ) {     
    storedCard[i] = EEPROM.read(start + i);   
  }
}

boolean checkTwo ( byte a[], byte b[] ) {
  if ( a[0] != NULL )       
    match = true;       
  for ( int k = 0; k < 4; k++ ) {  
    if ( a[k] != b[k] )     
      match = false;
  }
  if ( match ) {      
    return true;      
  }
  else  {
    return false;       
  }
}

int findIDSLOT( byte find[] ) {
  int count = EEPROM.read(0);      
  for ( int i = 1; i <= count; i++ ) {    
    readID(i);                
    if ( checkTwo( find, storedCard ) ) {   
      return i;       
      break;          
    }
  }
}

boolean findID( byte find[] ) {
  int count = EEPROM.read(0);     
  for ( int i = 1; i <= count; i++ ) {    
    readID(i);          
    if ( checkTwo( find, storedCard ) ) {   
      return true;
      break;  
    }
    else {   
    }
  }
  return false;
}

boolean isMaster( byte test[] ) {
  if ( checkTwo( test, masterCard ) )
    return true;
  else
    return false;
}

void digitalClockDisplay()
{
  Serial.print(hour());
  printDigits(minute());
  //printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


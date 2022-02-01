/* This Code Is For Biometric Door Unlock
   Press The Enroll / Verify Switch and
   Follow The Instruction Given On LCD 
   Upon Successful authentication of fp
   Solenoid Get Unlock For 2 Sec and Get Lock
   Author: Mohammed Aslam
   Embedded Engineer    
*/
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define en_sw    4
#define ver_sw   5
#define relay    6
#define buz  11

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
bool ch_status=false;
int e,v,flag1=0,flag2=0;
uint8_t id=0;
int getFingerprintIDez();

void setup()  
{
  pinMode(en_sw,INPUT_PULLUP);
  pinMode(ver_sw,INPUT_PULLUP);
  pinMode(buz,OUTPUT);
  pinMode(relay,OUTPUT);
  digitalWrite(relay,1);
  digitalWrite(buz,0);
  digitalWrite(buz,1);
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print(F("WELCOME"));
  lcd.setCursor(0, 1);
  lcd.print(F(" .............. "));
  
  delay(1000);
  lcd.clear();  
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println(F("\n\nAdafruit Fingerprint sensor enrollment & Verify"));
  finger.begin(9600); // set fingerprint baud-rate as per your fp module
  
  if (finger.verifyPassword()) {
    Serial.println(F("Found fingerprint sensor!"));
      lcd.setCursor(0, 0);
      lcd.print(F("FOUND FP SENSOR "));delay(1500);

  } else {
    Serial.println(F("Did not find fingerprint sensor :("));
    lcd.setCursor(0, 0);
      lcd.print(F("FP NOT FOUND  "));
    while (1) { delay(1); }
  }
 
 delay(1000);
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print(F("FINGRPRINT BASED "));
 lcd.setCursor(0,1);
 lcd.print(F("DOOR UNLOCK SYS. "));
  
 delay(2000);
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print(F("PLS SCAN UR FINGER"));
 lcd.setCursor(0,1);
 lcd.print(F("TO UNLOCK DOOR    "));
}

uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void lcd_msg(){
 delay(2000);
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print(F("PLS SCAN UR FINGER"));
 lcd.setCursor(0,1);
 lcd.print(F("TO UNLOCK DOOR    ")); 
}

void open_close(){
   delay(2000);
   lcd.setCursor(0,1);                                
   lcd.print(F("DOOR UNLOCK     "));            
   digitalWrite(relay,0);
                delay(2000);
   lcd.setCursor(0,1);                                
   lcd.print(F("DOOR LOCK       "));            
   digitalWrite(relay,1);
   lcd_msg();
}

void loop()                     // run over and over again
{
  e=digitalRead(en_sw);
  v=digitalRead(ver_sw);
if(e==0){
  if(ch_status==false){
  id++;
  
      lcd.setCursor(0,0);
      lcd.print(F("Enrolling......."));
      Serial.print(F("Enrolling ID #"));
      Serial.println(id);
  // enroll process place your finger on the fingerprint module 
  // remove finger from fingerprint module when it say remove 
  // place same finger again to complete enroll process 
  // LCD display ENROLL SUCCESS along with ID number
      p=getFingerprintEnroll();   
}

if(v==0) {
   if(flag2<3){
            lcd.setCursor(0,0);
            lcd.print(F("Ver. Finger To Acc"));
            delay(2000);
            finger.getTemplateCount();
            Serial.print(F("Sensor contains ")); Serial.print(finger.templateCount); 
            Serial.println(F(" templates"));
            Serial.println(F("Waiting for valid finger...")); 
            int ID;
            ID=getFingerprintIDez();//returns  Finger Id
            delay(1500);            //don't ned to run this at full speed.
          
       if(ID==1){
            Serial.println(F("Match Found With ID"));
            digitalWrite(buz,0);
            lcd.setCursor(0,1);                                
            lcd.print(F("MATCH FOUND ID 1"));            
            open_close();
            }   
          else if(ID==2){
            Serial.println(F("Match Found With ID"));
            digitalWrite(buz,0);
            lcd.setCursor(0,1);                                
            lcd.print(F("MATCH FOUND ID 2"));
            open_close();
            } 
          else if(ID==3){
            Serial.println(F("Match Found With ID"));
            digitalWrite(buz,0);            
            lcd.setCursor(0,1);                                
            lcd.print(F("MATCH FOUND ID 3"));
            open_close();
            }
          else if(ID==4){
            Serial.println(F("Match Found With ID"));            
            digitalWrite(buz,0);            
            lcd.setCursor(0,1);                                
            lcd.print(F("MATCH FOUND ID 4")); 
            open_close();
            }
          else{
            flag2++;
            Serial.println(F("Match Not Found"));
            digitalWrite(buz,1);//delay(1000);
            digitalWrite(buz,0);//delay(200);
            lcd.setCursor(0,0);
            lcd.print(F("PLS. TRY AGAIN    "));
            lcd.setCursor(0,1);                                
            lcd.print(F("MATCH NOT FOUND"));delay(1000);
            lcd.setCursor(0,0);
              lcd.print(F("PLS SCAN UR FINGER"));
              lcd.setCursor(0,1);
              lcd.print(F("                   "));
            }     
      
  if(flag2==3){
    Serial.println("Exceed Limit");delay(500);
    lcd.setCursor(0,0);                                
    lcd.print(F("EXCEED FP SCAN"));
    lcd.setCursor(0,0);                                
    lcd.print(F("LIMITS"));
    lcd_msg();
    //break;
  }
  }// */ 
  }
} // Loop End
/////////////////////////////////////////////////////////////////////////                          

//////////////  Enroll   ///////////////////

uint8_t getFingerprintEnroll() {

  
  Serial.print(F("Waiting for valid finger to enroll as #")); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image taken"));
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(F("."));
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println(F("Imaging error"));
      break;
    default:
      Serial.println(F("Unknown error"));
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image converted"));
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(F("Image too messy"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    default:
      Serial.println(F("Unknown error"));
      return p;
  }
  
  Serial.println(F("Remove finger"));
  lcd.setCursor(0,0);
  lcd.print(F("Remove Finger   "));
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print(F("ID ")); Serial.println(id);
  p = -1;
  Serial.println(F("Place same finger again"));
 // lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Put Finger Again"));
  delay(3000);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image taken"));
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(F("."));
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println(F("Imaging error"));
      break;
    default:
      Serial.println(F("Unknown error"));
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image converted"));
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(F("Image too messy"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    default:
      Serial.println(F("Unknown error"));
      return p;
  }
  
  // OK converted!
  Serial.print(F("Creating model for #"));  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Prints matched!"));
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(F("Communication error"));
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println(F("Fingerprints did not matches"));
    delay(1000);
    lcd.setCursor(0,0);
    lcd.print(F("FAIL TO ENROLL  "));id--;delay(1500);
    return p;
  } else {
    Serial.println(F("Unknown error"));
    return p;
  }   
  
  Serial.print(F("ID ")); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Stored!"));
 //   lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Enroll Success "));lcd.print(id);delay(1000);lcd_msg();return p;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(F("Communication error"));
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println(F("Could not store in that location"));
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println(F("Error writing to flash"));
    return p;
  } else {
    Serial.println(F("Unknown error"));
    return p;
  }   
}
//////////////  Enroll End  ///////////////////

//////////////   Verify    ///////////////////

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image taken"));
      lcd.setCursor(0,1);                                
      lcd.print(F("Image Taken"));
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(F("No finger detected"));
      lcd.setCursor(0,1);                                
      lcd.print(F("NO FINGER      "));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println(F("Imaging error"));
      return p;
    default:
      Serial.println(F("Unknown error"));
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image converted"));
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(F("Image too messy"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    default:
      Serial.println(F("Unknown error"));
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Found a print match!"));
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(F("Communication error"));
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println(F("Did not find a match"));
    return p;
  } else {
    Serial.println(F("Unknown error"));
    return p;
  }   
  
  // found a match!
  Serial.print(F("Found ID #")); Serial.print(finger.fingerID); 
  Serial.print(F(" with confidence of ")); Serial.println(finger.confidence); 

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print(F("Found ID #")); Serial.print(finger.fingerID); 
  Serial.print(F(" with confidence of ")); Serial.println(finger.confidence);
  return finger.fingerID; 
}
////////////// Verify End ////////////////////

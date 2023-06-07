/* Typical pin layout used:
 * ---------------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino        Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega           Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin            Pin        Pin              Pin
 * ---------------------------------------------------------------------------------------------
 * RST/Reset   RST          9             12(Black)      D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53(White)      D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51(Green)      D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50(Yellow)     D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52(Red)        D13        ICSP-3           15
 * VCC         3.3V         3.3V          3.3V(Orange)        
 * GND         GND          GND           GND(Brown)
 */
/*
   Servo Connections
   Motor       Arduino Pin    Mega Pins
   1           3              2
   2           5              3
   3           6              10
   4           not defined    11
*/
/* 
   IR Connections
   IR Sensor     Mega Pins
   1             24
   2             25
   3             26
   4             27
*/
/*
   LCD Connections
   Lcd pin       Mega Pin
   1             Gnd
   2             Vcc
   3             Potentiometer Midpoint
   4             8
   6             9
   11            4
   12            5
   13            6
   14            7
   15            Vcc
   16            Gnd
*/

/*
NodeMCU - Arduino Mega Connections

Mega                      NodeMCU
30                           D1
31                           D2
32                           D3
33                           D4
 
*/

/*
  MEGA PIN 45 Used to Start the Vehicle 
*/
 
#include<SPI.h>
#include<MFRC522.h>
#include<Servo.h>
#include<LiquidCrystal.h>
#define RST_PIN         12          // Configurable, see typical pin layout above
#define SS_PIN          53         // Configurable, see typical pin layout above

Servo servo1;  // create servo object to control servo1
Servo servo2;  // create servo object to control servo2
Servo servo3;  // create servo object to control servo3
Servo servo4;  // create servo object to control servo4

int pos1 = 0;    // variable to store the servo1 position
int pos2 = 0;    // variable to store the servo2 position
int pos3 = 0;    // variable to store the servo3 position
int pos4 = 0;    // variable to store the servo4 position

MFRC522 mfrc522(SS_PIN, RST_PIN);

int i=0;
int c=0;

byte Read_id[3];

LiquidCrystal lcd(8,9,4,5,6,7);// initialize the library by providing the number of pins to it

void setup() {
    
    servo1.attach(2);  // attaches the servo1 on pin 2 to the servo object
    servo2.attach(3);  // attaches the servo2 on pin 3 to the servo object
    servo3.attach(10);  // attaches the servo3 on pin 10 to the servo object
    servo4.attach(11);  // attaches the servo4 on pin 11 to the servo object 

    servo1.write(210); // servo1 initial position
    servo2.write(150); // servo2 initial position
    servo3.write(0);   // servo3 initial position
    servo4.write(0);   // servo4 initial position

    pinMode(24,INPUT); // Input of IR sensor_1
    pinMode(25,INPUT); // Input of IR sensor_2
    pinMode(26,INPUT); // Input of IR sensor_3
    pinMode(27,INPUT); // Input of IR sensor_4

    pinMode(30,OUTPUT); // Set high when Box1 closes:- Signaling NodeMCU
    pinMode(31,OUTPUT); // Set high when Box2 closes:- Signaling NodeMCU
    pinMode(32,OUTPUT); // Set high when Box3 closes:- Signaling NodeMCU
    pinMode(33,OUTPUT); // Set high when Box4 closes:- Signaling NodeMCU

    pinMode(45,OUTPUT); // Signal to start the vehicle

    Serial.begin(9600);   // Initialize serial communications with the PC
    while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();      // Init SPI bus

    lcd.begin(16,2);// Initialise LCD

 
    mfrc522.PCD_Init();   // Init MFRC522
 
    mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
    //Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
   
    Serial.println("Scan Card");
    lcd.setCursor(0,0); // Set cursor to 0,0 position
    lcd.print("SCAN CARD");
  
  
}

void loop() 
{
if(!mfrc522.PICC_IsNewCardPresent())
{
  return;
} 
if(!mfrc522.PICC_ReadCardSerial())
{
  return ;
}
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

Read_id[0]=mfrc522.uid.uidByte[0];
Read_id[1]=mfrc522.uid.uidByte[1];
Read_id[2]=mfrc522.uid.uidByte[2];
Read_id[3]=mfrc522.uid.uidByte[3];

/*Serial.println("Card uid is  ");
delay(1000);
for ( i=0;i<4;i++)
  Serial.println(mfrc522.uid.uidByte[i],HEX);
  delay(1000);*/
  
  // Compare Card Uid with stored Uid
  
 // For box 1
  if ( mfrc522.uid.uidByte[0] == 0x93 && 
       mfrc522.uid.uidByte[1] == 0x37 &&
       mfrc522.uid.uidByte[2] == 0x99 &&
       mfrc522.uid.uidByte[3] == 0x1A)
     {
      
      lcd.clear();
      lcd.setCursor(0,0);
      Serial.println("Opening Box 1");
      lcd.print("Opening Box 1");
      delay(1000);
      
      for (pos1 = 210; pos1 >= 80; pos1 -= 1) { // goes from 210 degrees to 80 degrees
      // in steps of 1 degree
      servo1.write(pos1);              // tell servo1 to go to position in variable 'pos1'
      delay(15);                       // waits 15ms for the servo1 to reach the position
                                         }
     
      {
        while(digitalRead(24)==0);
      }
      delay(2000);
      digitalWrite(30,HIGH);   //Turn High when Box1 closes
      delay(3000);
      digitalWrite(30,LOW);
      delay(2000);  // delay between open and close

      lcd.clear();
      lcd.setCursor(0,0);
      Serial.println("Closing Box 1");
      lcd.print("Closing Box 1");
      
      delay(1000);
      for (pos1 = 80; pos1 <= 210; pos1 += 1) { // goes from 80 degrees to 210 degrees
      servo1.write(pos1);              // tell servo1 to go to position in variable 'pos1'
      delay(15);                       // waits 15ms for the servo1 to reach the position
                                         } 
                                         
      delay(1000);

      digitalWrite(45,HIGH);// Signal to start the vehicle
      delay(3000);
      digitalWrite(45,LOW);

      lcd.clear();
      lcd.setCursor(0,0);
      Serial.println("Scan Card");
      lcd.print("SCAN CARD");
      
     }
 
 // For box 2
 else if(mfrc522.uid.uidByte[0] == 0x93 && 
         mfrc522.uid.uidByte[1] == 0xE4 &&
         mfrc522.uid.uidByte[2] == 0xB1 &&
         mfrc522.uid.uidByte[3] == 0x1A)
     {
      
      lcd.clear();
      lcd.setCursor(0,0);
      Serial.println("Opening Box 2");
      lcd.print("Opening Box 2");
      delay(1000);
      
      for (pos2 = 150; pos2 >= 50; pos2 -= 1) { // goes from 150 degrees to 50 degrees
      // in steps of 1 degree
      servo2.write(pos2);              // tell servo2 to go to position in variable 'pos2'
      delay(15);                       // waits 15ms for the servo2 to reach the position
                                         }
      
      {
        while(digitalRead(25)==0);
      }
      delay(2000);
      digitalWrite(31,HIGH);   //Turn High when Box2 closes
      delay(3000);
      digitalWrite(31,LOW);
      delay(2000);  // delay between open and close

      lcd.clear();
      lcd.setCursor(0,0);
      Serial.println("Closing Box 2");
      lcd.print("Closing Box 2");
      
      delay(1000);
      for (pos2 = 50; pos2 <= 150; pos2 += 1) { // goes from 50 degrees to 150 degrees
      servo2.write(pos2);              // tell servo2 to go to position in variable 'pos2'
      delay(15);                       // waits 15ms for the servo2 to reach the position
                                         } 
                                         
      delay(1000);

      digitalWrite(45,HIGH);// Signal to start the vehicle
      delay(3000);
      digitalWrite(45,LOW);

      lcd.clear();
      lcd.setCursor(0,0);
      Serial.println("Scan Card");
      lcd.print("SCAN CARD");
      
     }
 
 // For box 3
 else if(mfrc522.uid.uidByte[0] == 0x93 && 
         mfrc522.uid.uidByte[1] == 0x34 &&
         mfrc522.uid.uidByte[2] == 0xE9 &&
         mfrc522.uid.uidByte[3] == 0x1A)
     {

      lcd.clear();
      lcd.setCursor(0,0);
      Serial.println("Opening Box 3");
      lcd.print("Opening Box 3");
      delay(1000);
      
      for (pos3 = 0; pos3 <= 110; pos3 += 1) { // goes from 0 degrees to 110 degrees
      // in steps of 1 degree
      servo3.write(pos3);              // tell servo3 to go to position in variable 'pos3'
      delay(15);                       // waits 15ms for the servo3 to reach the position
                                         }
      
      {
        while(digitalRead(26)==0);
      }
      delay(2000);
      digitalWrite(32,HIGH);   //Turn High when Box3 closes
      delay(3000);
      digitalWrite(32,LOW);
      delay(2000);  // delay between open and close
      
      
      lcd.clear();
      lcd.setCursor(0,0);
      Serial.println("Closing Box 3");
      lcd.print("Closing Box 3");

      delay(1000);
      for (pos3 = 110; pos3 >= 0; pos3 -= 1) { // goes from 180 degrees to 0 degrees
      servo3.write(pos3);              // tell servo3 to go to position in variable 'pos3'
      delay(15);                       // waits 15ms for the servo3 to reach the position
                                         } 
                                         
      delay(1000);

      digitalWrite(45,HIGH);// Signal to start the vehicle
      delay(3000);
      digitalWrite(45,LOW);

      lcd.clear();
      lcd.setCursor(0,0);
      Serial.println("Scan Card");
      lcd.print("SCAN CARD");
      
     }


// For box 4
 else if(mfrc522.uid.uidByte[0] == 0xA3 && 
         mfrc522.uid.uidByte[1] == 0x5E &&
         mfrc522.uid.uidByte[2] == 0x45 &&
         mfrc522.uid.uidByte[3] == 0x1A)
     {

      lcd.clear();
      lcd.setCursor(0,0);
      Serial.println("Opening Box 4");
      lcd.print("Opening Box 4");
      delay(1000);

      for (pos4 = 0; pos4 <= 70; pos4 += 1) { // goes from 0 degrees to 70 degrees
      // in steps of 1 degree
      servo4.write(pos4);              // tell servo4 to go to position in variable 'pos4'
      delay(15);                       // waits 15ms for the servo4 to reach the position
                                         }
      
      {
        while(digitalRead(27)==0);
      }
      delay(2000);
      digitalWrite(33,HIGH);   //Turn High when Box4 closes
      delay(3000);
      digitalWrite(33,LOW);
      delay(2000);  // delay between open and close

      lcd.clear();
      lcd.setCursor(0,0);
      Serial.println("Closing Box 4");
      lcd.print("Closing Box 4");
      
      delay(1000);
      for (pos4 = 70; pos4 >= 0; pos4 -= 1) { // goes from 70 degrees to 0 degrees
      servo4.write(pos4);              // tell servo4 to go to position in variable 'pos4'
      delay(15);                       // waits 15ms for the servo4 to reach the position
                                         } 
                                         
      delay(1000);

      digitalWrite(45,HIGH);// Signal to start the vehicle
      delay(3000);
      digitalWrite(45,LOW);

      lcd.clear();
      lcd.setCursor(0,0);
      Serial.println("Scan Card");
      lcd.print("SCAN CARD");
      
     }


// Master Card
 else if(Read_id[0] == 0x91 && 
         Read_id[1] == 0x3E &&
         Read_id[2] == 0x8A &&
         Read_id[3] == 0x2F &&
         c==0)
     {
      
     
      lcd.clear();
      lcd.setCursor(0,0);
      Serial.println("Opening All Boxes");
      lcd.print("Opening All ");
      lcd.setCursor(0,1);
      lcd.print("  Boxes  ");

      delay(1000);
      
      for (pos1 = 210; pos1 >= 80; pos1 -= 1) { // goes from 210 degrees to 80 degrees
      // in steps of 1 degree
      servo1.write(pos1);              // tell servo1 to go to position in variable 'pos1'
      delay(15);                       // waits 15ms for the servo1 to reach the position
                                         }

      for (pos2 = 150; pos2 >= 50; pos2 -= 1) { // goes from 150 degrees to 50 degrees
      // in steps of 1 degree
      servo2.write(pos2);              // tell servo2 to go to position in variable 'pos2'
      delay(15);                       // waits 15ms for the servo2 to reach the position
                                         }   

      for (pos3 = 0; pos3 <= 110; pos3 += 1) { // goes from 0 degrees to 110 degrees
      // in steps of 1 degree
      servo3.write(pos3);              // tell servo3 to go to position in variable 'pos3'
      delay(15);                       // waits 15ms for the servo3 to reach the position
                                         }     

      for (pos4 = 0; pos4 <= 70; pos4 += 1) { // goes from 0 degrees to 70 degrees
      // in steps of 1 degree
      servo4.write(pos4);              // tell servo4 to go to position in variable 'pos4'
      delay(15);                       // waits 15ms for the servo4 to reach the position
                                         }  

      delay(2000);

      lcd.clear();
      lcd.setCursor(0,0);
      Serial.println("Scan Card");
      lcd.print(" SCAN CARD To ");
      lcd.setCursor(0,1); 
      lcd.print("Close All Boxes "); 

      c=1;

      

     }
     


 else if(Read_id[0] == 0x91 && 
         Read_id[1] == 0x3E &&
         Read_id[2] == 0x8A &&
         Read_id[3] == 0x2F &&
         c==1)      
     { 
      
      
      lcd.clear();
      lcd.setCursor(0,0);
      Serial.println("Closing All Boxes");
      lcd.print(" Closing All ");
      lcd.setCursor(0,1);
      lcd.print("  Boxes  ");

      delay(1000);
     
      for (pos1 = 80; pos1 <= 210; pos1 += 1) { // goes from 80 degrees to 210 degrees
      servo1.write(pos1);              // tell servo1 to go to position in variable 'pos1'
      delay(15);                       // waits 15ms for the servo1 to reach the position
                                         } 
      
      for (pos2 = 50; pos2 <= 150; pos2 += 1) { // goes from 50 degrees to 150 degrees
      servo2.write(pos2);              // tell servo2 to go to position in variable 'pos2'
      delay(15);                       // waits 15ms for the servo2 to reach the position
                                         } 

      for (pos3 = 110; pos3 >= 0; pos3 -= 1) { // goes from 110 degrees to 0 degrees
      servo3.write(pos3);              // tell servo3 to go to position in variable 'pos3'
      delay(15);                       // waits 15ms for the servo3 to reach the position
                                         } 

      for (pos4 = 70; pos4 >= 0; pos4 -= 1) { // goes from 70 degrees to 0 degrees
      servo4.write(pos4);              // tell servo4 to go to position in variable 'pos4'
      delay(15);                       // waits 15ms for the servo4 to reach the position
                                         } 

      delay(2000);

      digitalWrite(45,HIGH);// Signal to start the vehicle
      delay(3000);
      digitalWrite(45,LOW);

      lcd.clear();
      lcd.setCursor(0,0);
      Serial.println("Scan Card");
      lcd.print("SCAN CARD");

      c=0;

                                                                                                     
     }
     
     

         
// Invalid case    
 else {
         lcd.clear();
         lcd.setCursor(0,0);
         Serial.println("Invalid Card");
         lcd.print("Invalid Card");
         delay(3000);
         
         lcd.clear();
         lcd.setCursor(0,0);
         Serial.println("Scan Card");
         lcd.print("SCAN CARD");
         
      }

}

#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Servo.h>
Servo servo1;
int servoPin = 13;

#define SS_PIN 48
#define RST_PIN 49
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
  int b=0;
int s=0;
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
    servo1.attach(servoPin);
  // set up the LCD's number of columns and rows:
 pinMode(A5, OUTPUT); //Konfiguracja A5 jako wyjście
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Przyloz karte!");
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  servo1.write(130);
}

void loop() {
  if(s==1){
servo1.write(90);
s=0;
delay(1000);
servo1.write(180);
  }
  
   if(b>2)
  { tone(A5, 1000); //Wygeneruj sygnał o częstotliwości 1000Hz na pinie A5  
  }
 if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "59 E5 12 E9" || content.substring(1) == "8A A6 DE 84" ) //change here the UID of the card/cards that you want to give access
  {
    noTone(A5);
     b=0;
    Serial.println("Authorized access");
     lcd.setCursor(0, 1);
     lcd.print("Access ");
    servo1.write(90);
  delay(1000);
  s=1;
    Serial.println();
    delay(3000);
  }
 
 else   {
    Serial.println(" Access denied");
     lcd.setCursor(0, 1);
      lcd.print("Denied");
      b=b+1;
    delay(3000);
    
  }
  
  }

#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h> //biblioteka od klawiatury
#include <LiquidCrystal.h>
#include <Servo.h>
#include <arduino-timer.h>
#define SS_PIN 48
#define RST_PIN 49
MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myservo;

Servo servo1;
bool timerfinis = false;
int servoPin = 13;
auto timer = timer_create_default();
auto task = timer.in(0, [](void *) -> void {});
int b = 0;
int kk = 0;
int s = 0;
const byte ROWS = 4; // ile wierszy
const byte COLS = 4; // ile kolumn
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
byte rowPins[ROWS] = {26, 27, 28, 29}; // piny wierszy
byte colPins[COLS] = {25, 24, 23, 22}; // piny kolum

char keys[ROWS][COLS] = { // mapowanie klawiatury
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); // inicjalizacja klawiatury
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int cursorColumn = 0;

#define Password_Lenght 7
char Data[Password_Lenght]; 

char Master[Password_Lenght] = "123456";
int pos = 0;
byte data_count = 0, master_count = 0;
bool Pass_is_good;
bool door = true;
char customKey;
int jakas = 0;

void setup()
{
    myservo.attach(13);
    ServoClose();
    Serial.begin(9600);
    lcd.begin(16, 2);
    SPI.begin();
    mfrc522.PCD_Init();
    lcd.print(" Projekt zamek");
    lcd.setCursor(0, 1);
    lcd.print(":)");
    delay(3000);
    lcd.clear();
    pinMode(A5, OUTPUT); // Konfiguracja A5 jako wyjście
    lcd.begin(16, 2);
    // Print a message to the LCD.
    mfrc522.PCD_Init(); // Initiate MFRC522
    Serial.println("Approximate your card to the reader...");
    Serial.println();
    servo1.write(130);
    lcd.setCursor(0, 0);
    lcd.print("PODAJ K/H");
}
void loop()
{
    timer.tick();
    if (b > 2)
    {
        tone(A5, 1000); // Wygeneruj sygnał o częstotliwości 1000Hz na pinie A5
        Open2();
    }

    else if (kk > 2)
    {
        tone(A5, 1000); // Wygeneruj sygnał o częstotliwości 1000Hz na pinie A5
        if (mfrc522.PICC_IsNewCardPresent())
        {
            karta2();
            clearData();
        }
    }

    else if (door == 0)
    {
        customKey = customKeypad.getKey();

        if (customKey == '#' || timerfinis)
        {

            lcd.clear();

            ServoClose();

            lcd.print(" ZAMEK ZAMKNIETY");

            delay(3000);
            lcd.clear();

            door = 1;
            timer.cancel(task);
            lcd.print(" PODAJ K/H");
        }
    }
    else if (mfrc522.PICC_IsNewCardPresent())
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        karta();
        clearData();
    }
    else
        Open();
}

void clearData()
{
    for (; data_count != 0;)
    {
        Data[data_count--] = 0; // clear array for new data
    }

    return;
}

void ServoOpen()
{
    for (pos = 180; pos >= 0; pos -= 5)
    { // goes from 0 degrees to 180 degrees

        // in steps of 1 degree

        myservo.write(pos); // tell servo to go to position in variable 'pos'
        delay(15);          // waits 15ms for the servo to reach the position
    }
    timerfinis = false;
    task = timer.in(10000, czas);
}

void ServoClose()
{
    for (pos = 0; pos <= 180; pos += 5)
    {                       // goes from 180 degrees to 0 degrees
        myservo.write(pos); // tell servo to go to position in variable 'pos'

        delay(15); // waits 15ms for the servo to reach the position
    }
}

void Open()
{
    lcd.setCursor(0, 0);
    Serial.println("bla");

    customKey = customKeypad.getKey();

    if (customKey) // makes sure a key is actually pressed, equal to (customKey != NO_KEY)
    {
        switch (customKey)
        {
        case 'B':

            clearData();

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(" PODAJ K/H");
            lcd.setCursor(0, 1);
            break;
        default:
            jakas++;
            Data[data_count] = customKey; 

            lcd.setCursor(data_count, 1); 

            lcd.print(Data[data_count]); 

            data_count++; 
            break;
        }
    }

    if (data_count == Password_Lenght - 1)
    {
        if (!strcmp(Data, Master)) 
        {
            lcd.clear();

            ServoOpen();

            lcd.print(" ZAMEK OTWARTY");
            kk = 0;
            door = 0;
        }
        else
        {
            lcd.clear();

            lcd.print(" ZLE HASLO");
            kk = kk + 1;
            delay(1000);

            door = 1;
            lcd.clear();
            if (kk > 2)
                lcd.print(" PRZYLOZ KARTE");
            else
                lcd.print(" PODAJ K/H");
        }

        clearData();
    }
}

void karta()
{
    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }
    // Show UID on serial monitor
    Serial.print("UID tag :");
    String content = "";
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
    if (content.substring(1) == "59 E5 12 E9" || content.substring(1) == "8A A6 DE 84") 
    {
        b = 0;
        lcd.setCursor(0, 0);
        lcd.print(" ZAMEK OTWARTY ");
        ServoOpen();
        door = 0;
    }

    else
    {
        lcd.setCursor(0, 0);
        lcd.print(" BLAD KARTY");
        b = b + 1;
        delay(1000);
        lcd.clear();
        lcd.print(" PODAJ K/H");
    }
}
void czas(void *)
{
    timerfinis = true;
    return;
}

void Open2()
{
    lcd.setCursor(0, 0);
    lcd.print(" PODAJ PIN");
    customKey = customKeypad.getKey();

    if (customKey) 
    {
        switch (customKey)
        {
        case 'B':

            clearData();

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(" PODAJ PIN");
            lcd.setCursor(0, 1);
            break;
        default:
            jakas++;
            Data[data_count] = customKey; 

            lcd.setCursor(data_count, 1); 

            lcd.print(Data[data_count]); 

            data_count++; 
            break;
        }
    }

    if (data_count == Password_Lenght - 1) // 
    {
        if (!strcmp(Data, Master)) 
        {
            lcd.clear();
            ServoOpen();

            lcd.print(" ZAMEK OTWARTY");
            door = 0;
            b = 0;
            noTone(A5);
        }
        else
        {
            lcd.clear();

            lcd.print(" ZLE HASLO");

            delay(1000);

            door = 1;
            lcd.clear();
            lcd.print(" PODAJ K/H");
        }

        clearData();
    }
}

void karta2()
{
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }
    Serial.print("UID tag :");
    String content = "";
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
    if (content.substring(1) == "59 E5 12 E9" || content.substring(1) == "8A A6 DE 84") 
    {
        kk = 0;
        noTone(A5);
        lcd.setCursor(0, 0);
        lcd.print(" ZAMEK OTWARTY ");
        ServoOpen();
        door = 0;
    }

    else
    {
        lcd.clear();
        lcd.print(" BLAD KARTY");
        delay(1000);
        lcd.clear();
        lcd.print(" PRZYLOZ KARTE");
    }
}

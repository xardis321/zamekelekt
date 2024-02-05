#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define BUTTON_PIN 2
#define PIN 6       // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 8 // Popular NeoPixel ring size
#define PIXEL_PIN 6 // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 8
#define BUTTON_PIN2 8
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

boolean oldState = HIGH;
int mode = 0; // Currently-active animation mode, 0-9
int a, b;
int t[10];
int s[10];
int potencjometr = 0;
int odczytanaWartosc = 0;
boolean oldState2 = HIGH;
int mode2 = 0; // Currently-active animation mode, 0-9
int a2, b2,c2;
int t2[10];
int s2[10];
int potencjometr2 = 0;
int odczytanaWartosc2 = 0;

void wipe(int wait)
{
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
}
void wipe2(int wait)
{
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
}

void start(int wait)
{
 pixels.clear(); // Set all pixel colors to 'off'
  for(int i=0; i<NUMPIXELS; i++) { 
    a=(rand() > RAND_MAX/2) ? 100 : 150;
    if(a==100)
    {
    pixels.setPixelColor(i, pixels.Color(a,a,0)); 
    }
    
    if(a==150)
    {
     pixels.setPixelColor(i, pixels.Color(0,a,a)); 
    }

    t[i]=a;
     Serial.print(t[i]);
     Serial.print("\t"); 
     delay(wait); 
    pixels.show(); 
    
  }
  //koniec petli i wyzerowanie paska
 delay(700);
for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
  }

void start2(int wait)
{
   long rand;
   pixels.show();
 pixels.clear(); // Set all pixel colors to 'off'
  for(int i=0; i<8; i++) { 
   
  rand = random(1, 4);  // returns a value 1, 2, 3
  pixels.show(); 
   switch (rand) {
      case 1:
          pixels.setPixelColor(i, pixels.Color(a2,a2,0)); 
           a2=100;
           pixels.show(); 
         break;

     case 2:
        pixels.setPixelColor(i, pixels.Color(0,a2,a2)); 
         a2=150;
         pixels.show(); 
         break;


     case 3:
         pixels.setPixelColor(i, pixels.Color(a2,0,a2)); 
         a2=200;
         pixels.show(); 
         break;
    
   }
   pixels.show(); 
   

    t2[i]=a2; 
     Serial.print(t2[i]);
     Serial.print("\t"); 
     delay(wait); 
   
    
  }
  //koniec petli i wyzerowanie paska
 delay(1000);
for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
  }

  
 
      

void wybor(int wait)
{
  int iledow = 0;
  boolean nowy = LOW;
  boolean stary = LOW;
  pixels.clear(); // Set all pixel colors to 'off'
  while (iledow < 8)
  {
    nowy = digitalRead(BUTTON_PIN);
    if ((nowy == LOW) && (stary == HIGH))
    {
      iledow++;
    }
    else
    {
      delay(20);

      odczytanaWartosc = analogRead(A5) / 4;
      if (odczytanaWartosc > 0 && odczytanaWartosc < 150)
      {

        pixels.setPixelColor(iledow , pixels.Color(100, 100, 0));
        b = 100;
      }

      if (odczytanaWartosc > 150 && odczytanaWartosc < 300)
      {
        pixels.setPixelColor(iledow, pixels.Color(0, 150, 150));
        b = 150;
      }
      delay(wait);
      pixels.show();
    }
    stary = nowy;
    s[iledow] = b;
  }
  Serial.print("\n");
  for (int iledow = 0; iledow < 8; iledow++)
  {
    Serial.print(s[iledow]);
    Serial.print("\t");
    
  }
}

void wybor2(int wait)
{
  int iledow = 0;
  boolean nowy = LOW;
  boolean stary = LOW;
  pixels.clear(); // Set all pixel colors to 'off'
  while (iledow < 8)
  {
    nowy = digitalRead(BUTTON_PIN2);
    if ((nowy == LOW) && (stary == HIGH))
    {
      iledow++;
    }
    else
    {
      delay(20);

      odczytanaWartosc2 = analogRead(A5) / 4;
      if (odczytanaWartosc2 > 0 && odczytanaWartosc2 < 86)
      {

        pixels.setPixelColor(iledow , pixels.Color(100, 100, 0));
        b2 = 100;
      }

      if (odczytanaWartosc2 > 86 && odczytanaWartosc2 < 180)
      {
        pixels.setPixelColor(iledow, pixels.Color(0, 150,150));
        b2 = 150;
      }
       if (odczytanaWartosc2 > 180 && odczytanaWartosc2 < 300)
      {
        pixels.setPixelColor(iledow, pixels.Color(200, 0, 200));
        b2 = 200;
      }
      delay(wait);
      pixels.show();
    }
    stary = nowy;
    s2[iledow] = b2;
  }
  Serial.print("\n");
  for (int iledow = 0; iledow < 8; iledow++)
  {
    Serial.print(s2[iledow]);
    Serial.print("\t");
    
  }
}

void setup()
{
  randomSeed(analogRead(A1)); 
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'
  pixels.begin();
}

void loop()
{
  // Get current button state.
  boolean newState = digitalRead(BUTTON_PIN);
  boolean newState2 = digitalRead(BUTTON_PIN2);
  if ((newState == LOW) && (oldState == HIGH))
  {
    delay(20);

    newState = digitalRead(BUTTON_PIN);
    if (newState == LOW)
    { // Yes, still low
      if (++mode > 4)
        mode = 1; // Advance to next mode, wrap around after #8
      switch (mode)
      { // Start the new animation...
      case 0:
        wipe(100);
        break;
      case 1:
        wipe(100);
        break;
      case 2:
        start(500);
        break;
      case 3:
        wybor(10);
        break;
        case 4:
         
        int temp=0;
        Serial.print("\n");
        for(int i=0;i<8;i++){
if (t[i]!=s[i]){
 pixels.setPixelColor(i, pixels.Color(100, 0, 0));
           pixels.show();
           Serial.print("ZLE");
    Serial.print("\t");
}
else{
  pixels.setPixelColor(i, pixels.Color(0, 100, 0));
  pixels.show();
  Serial.print("DOBRZE");
    Serial.print("\t");
}
}
       
        
         
         
         Serial.print("\n");
          break;
        }
       
      }
    }
  
  // Set the last-read button state to the old state.
  oldState = newState;
if ((newState2 == LOW) && (oldState2 == HIGH))
  {
    delay(30);

    newState2 = digitalRead(BUTTON_PIN2);
    if (newState2 == LOW)
    { // Yes, still low
      if (++mode2 > 4)
        mode2 = 1; // Advance to next mode, wrap around after #8
      switch (mode2)
      { // Start the new animation...
      case 0:
        wipe2(100);
        break;
      case 1:
        wipe2(100);
        break;
      case 2:
        start2(500);
        break;
      case 3:
        wybor2(10);
        break;
        case 4:
        Serial.print("\n");
        for(int i=0;i<8;i++){
if (t2[i]!=s2[i]){
 pixels.setPixelColor(i, pixels.Color(100, 0, 0));
           pixels.show();
            Serial.print("ZLE");
    Serial.print("\t");
}
else{
  pixels.setPixelColor(i, pixels.Color(0, 100, 0));
  pixels.show();
   Serial.print("DOBRZE");
    Serial.print("\t");
}
       }
         Serial.print("\n");
          break;
        }
       
      }
    }
  
  oldState2 = newState2;
}


 

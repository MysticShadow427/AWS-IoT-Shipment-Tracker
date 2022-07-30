#include <TinyGPSPlus.h>
#include "thingProperties.h" // code generarted by Arduino IoT Cloud for itself
#include <SoftwareSerial.h>
#include <SPI.h>
#include <WiFi101.h>
#include <LiquidCrystal.h>

#define LCD_RS_PIN A4
#define LCD_E_PIN A5
#define LCD_D4_PIN 2
#define LCD_D5_PIN 3
#define LCD_D6_PIN 4
#define LCD_D7_PIN 5

int status = WL_IDLE_STATUS;     // the WiFi radio's status


static const int RXPin = 9, TXPin = 10;
static const uint32_t GPSBaud = 4800;
int triggerLEDPin=7;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

LiquidCrystal lcd(LCD_RS_PIN, LCD_E_PIN, LCD_D4_PIN,
                            LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN); // craeting an LCD object

void setup()
{
  lcd.begin(16,2);
  lcd.print("Starting...");
  delay(1000);
  lcd.clear();
   // Defined in thingProperties.h
  initProperties();
  pinMode(triggerLEDPin,OUTPUT);

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  Serial.begin(115200);
  while(!Serial){};
  while(Serial.available()>0){
    Serial.read();
  }
  ss.begin(GPSBaud);

    /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

   // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    lcd.print("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  // as arduino needs to connect to a wifi network and thats must be open  
  while ( status != WL_CONNECTED) {
    lcd.print("Attempting to connect to open SSID: ");
    lcd.println(ssid);
    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  lcd.print("You're connected to the network");
  }

void loop()
{
  ArduinoCloud.update();
  onTriggerPinChange();
 // static const double DEST_LAT = 51.508131, DEST_LON = -0.128002;// we can give our destination coordinates accordingly

}

/*
  Since TriggerPin is READ_WRITE variable, onTriggerPinChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onTriggerPinChange()  {
  // Add your code here to act upon TriggerPin change
  //lcd print state of our boolean variable
  if(triggerPin){
    digitalWrite(triggerLEDPin,HIGH);// a led is used just as an indicator
   Serial.println("Sending data"); // sending data to RPi,this is an indicator to RPi to recieve data
   lcd.print("Sending data to RPi...");
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  delay(5000); // we will send lattitude and longtitude every 5 seconds for neat info passing through serial comm b/w Arduino and RPi
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  delay(5000);
  printDateTime(gps.date, gps.time);
 
  }
  else{
    digitalWrite(triggerLEDPin,LOW);
  }
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

//these are functions for conversions for the data to human readble data that the GPS functions generate

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.println(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.println(F("*"));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.println(sz); // date
  }
  
  if (!t.isValid())
  {
    Serial.println(F("*"));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.println(sz);
  }

  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}

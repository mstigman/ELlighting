#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h> 
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include <Time.h>


#ifdef __AVR__
  #include <avr/power.h>
#endif
#ifndef APSSID
#define APSSID "ELlighting"
#define APPSK  "wetakemanyels"
#endif

#define LEDPIN         14
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      30


int lightingState = 1; // 1 is normal, 2 is waves, 

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

const int brakeLightsStart = 12;
const int brakeLightsEnd = 17;

const int leftTurnStart = 0;
const int leftTurnEnd = 11;

const int rightTurnStart = 18;
const int rightTurnEnd = 29;

const int delayBrake = 300;
const int delayTurn = 500;

int gyroX;
int gyroY;
int gyroZ;

int accelX;
int accelY;
int accelZ;

int temp;

bool brake;
bool turnLeft;
bool turnRight;

int timeBrake;
int timeLeft;
int timeRight;

int indexColor;

time_t intervalUpdate;

time_t startTimeLights;
int pixelNumber = 0;

ESP8266WebServer server(80);

void setuplsm()
{
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_8G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
  
  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_12GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);

  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_500DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);
}


/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void querySensors() {

   /* creating a Json object */
   StaticJsonBuffer<200> jsonBuffer;
   JsonObject& root = jsonBuffer.createObject();

  root["acceleration x"] = accelX;
  root["acceleration y"] = accelY;
  root["acceleration z"] = accelZ;

  root["gyro x"] = gyroX;
  root["gyro y"] = gyroY;
  root["gyro z"] = gyroZ;

  root["temp"] = temp;
  String data;
  root.printTo(data);
  server.send(200, "application/json", data);
}

void lightingNormal() {
  lightingState = 1;
}

void lightingWaves() {
  lightingState = 2;
  startTimeLights = now();
  pixelNumber = 0;
}

void lightingFade() {
  lightingState = 3;
}

void brakingLightsOn() {
   for(int i = brakeLightsStart; i <= brakeLightsEnd; i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(255,0,0)); // Moderately bright green color.
  }

    pixels.show(); // This sends the updated pixel color to the hardware.
}

void brakingLightsOff() {
   for(int i = brakeLightsStart; i <= brakeLightsEnd; i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(25,0,0)); // Moderately bright green color.
  }

    pixels.show(); // This sends the updated pixel color to the hardware.
}

void rightTurnLightsOn() {
   for(int i = rightTurnStart; i <= rightTurnEnd; i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(255,255,0)); // Moderately bright green color.
  }

    pixels.show(); // This sends the updated pixel color to the hardware.
}

void rightTurnLightsOff() {
   for(int i = rightTurnStart; i <= rightTurnEnd; i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(100,100,100)); // Moderately bright green color.
  }

    pixels.show(); // This sends the updated pixel color to the hardware.
}

void leftTurnLightsOn() {
   for(int i = leftTurnStart; i <= leftTurnEnd; i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(255,255,0));
  }

    pixels.show(); // This sends the updated pixel color to the hardware.
}

void leftTurnLightsOff() {
   for(int i = leftTurnStart; i <= leftTurnEnd; i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(100,100,100)); // Moderately bright green color.
  }

    pixels.show(); // This sends the updated pixel color to the hardware.
}

void updateData() {
  lsm.read();  /* ask it to read in the data */   
  /* Get a new sensor event */ 
  sensors_event_t a, m, g, tempr;

  lsm.getEvent(&a, &m, &g, &tempr); 
  
  accelX = a.acceleration.x;
  accelY = a.acceleration.y;
  accelZ = a.acceleration.z;

  gyroX = g.gyro.x;
  gyroY = g.gyro.y;
  gyroZ = g.gyro.z;

  temp = tempr.temperature;
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

void setup() {
  delay(1000);
  Serial.begin(115200);

  ESP.eraseConfig();
  /* setting up sensors */
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
  }
  
  // helper to just set the default scaling we want, see above!
  setuplsm();

  
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/data", querySensors);
  server.on("/normal", lightingNormal);
  server.on("/waves", lightingWaves);
  server.on("/fade", lightingFade);
  server.begin();
  Serial.println("HTTP server started");

  pixels.begin(); // This initializes the NeoPixel library.

  brakingLightsOn();
  leftTurnLightsOn();
  rightTurnLightsOn();

  intervalUpdate = millis();//now();
  startTimeLights = millis();//now();
  
  brake = false;
  turnLeft = false;
  turnRight = false;
}



void loop() {
  server.handleClient();
  
  time_t t = millis();//now();
  
    updateData();
    if (lightingState == 1) {
      if (accelX > .75 || t < timeBrake + delayBrake) { 
        if(!brake) {
          brake = true;
          timeBrake = t;
        }
        if (accelX > .75 && t > timeBrake + 300) {
          brakingLightsOn();
          timeBrake = t;
        }
      } else {
        brakingLightsOff();
        brake = false;
      }
    
      if (accelY > 1 || t < timeRight + delayTurn) { 
        if(!turnRight) {
          turnRight = true;
          timeRight = t;
        }
        if (accelY > 1 && t > timeRight + 750) {
          rightTurnLightsOn();
          timeRight = t;
        }
      } else {
        rightTurnLightsOff();
        turnRight = false;
      }  
    
     
      if (accelY < -1 || t < timeLeft + delayTurn) { 
        if(!turnLeft) {
          turnLeft = true;
          timeLeft = t;
        }
        if (accelY < -1 && t > timeLeft + 750) {
          leftTurnLightsOn();
          timeLeft = t;
        }
      } else {
        leftTurnLightsOff();
        turnLeft = false;
      }  
      
    } else if (lightingState == 2) {
        if (t > startTimeLights + (250 / 1000)) {
          pixels.setPixelColor(pixelNumber, pixels.Color(0,0,0));
          pixelNumber++;
          if (pixelNumber == NUMPIXELS) {
            pixelNumber = 0;
          }
          pixels.setPixelColor(pixelNumber, pixels.Color(255,255,255));
          pixels.show();
        }
    } else if (lightingState == 3) {
         indexColor++;
         if (indexColor == 255) {
          indexColor = 0;
         }
         for(int i = 0; i <= NUMPIXELS; i++) {
            pixels.setPixelColor(i, Wheel(((i * 256 / NUMPIXELS) + indexColor) & 255));
         }
         pixels.show();
    }
}

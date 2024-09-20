#include <Adafruit_LIS3DH.h>

/*



*/

#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#define BUTTON_ONE 5
#define PIEZO A2
#define FLICKER A3
#define FLASH A4
#define COOL_ONE 13
#define COOL_TWO 12
#define COOL_THREE 11
#define COOL_FOUR 10
const char *ssid = "ArtifactDetector";
const char *password = "bruhdontconnecttothis556";
int off = 20;
int on = 150;
int toneFrequency = 4000;
int toneLength = 5;
bool CoolDown = false;
int CoolCount = 0;
int CoolIncrement = 10;
int i = 0;
bool FlashNow = false;

Adafruit_LIS3DH lis = Adafruit_LIS3DH();
Adafruit_NeoPixel Pixel(1, 33);

void setup() {
  Serial.begin(115200);
  Pixel.begin();
  Pixel.setPixelColor(0, 0, 255, 0);
  Pixel.show();
  pinMode(FLICKER, OUTPUT);
  pinMode(PIEZO, OUTPUT);
  pinMode(BUTTON_ONE, INPUT_PULLUP);
  pinMode(FLASH, OUTPUT);
  pinMode(COOL_ONE, OUTPUT);
  pinMode(COOL_TWO, OUTPUT);
  pinMode(COOL_THREE, OUTPUT);
  pinMode(COOL_FOUR, OUTPUT);
  digitalWrite(FLASH, LOW);
  digitalWrite(COOL_ONE, LOW);
  digitalWrite(COOL_TWO, LOW);
  digitalWrite(COOL_THREE, LOW);
  digitalWrite(COOL_FOUR, LOW);

  // We start by connecting to a WiFi network
  // To debug, please enable Core Debug Level to Verbose

  Serial.println();
  Serial.print("[WiFi] Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  // Auto reconnect is set true as default
  // To set auto connect off, use the following function
  //    WiFi.setAutoReconnect(false);

  // Will try for about 10 seconds (20x 500ms)
  int tryDelay = 500;
  int numberOfTries = 20;
  while (true) {

    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL: Serial.println("[WiFi] SSID not found"); break;
      case WL_CONNECT_FAILED:
        Serial.print("[WiFi] Failed - WiFi not connected! Reason: ");
        return;
        break;
      case WL_CONNECTION_LOST: Serial.println("[WiFi] Connection was lost"); break;
      case WL_SCAN_COMPLETED: Serial.println("[WiFi] Scan is completed"); break;
      case WL_DISCONNECTED: Serial.println("[WiFi] WiFi is disconnected"); break;
      case WL_CONNECTED:
        Serial.println("[WiFi] WiFi is connected!");
        Serial.print("[WiFi] IP address: ");
        Serial.println(WiFi.localIP());
        return;
        break;
      default:
        Serial.print("[WiFi] WiFi Status: ");
        Serial.println(WiFi.status());
        break;
    }
    delay(tryDelay);

    if (numberOfTries <= 0) {
      Serial.print("[WiFi] Failed to connect to WiFi!");
      // Use disconnect function to force stop trying to connect
      WiFi.disconnect();
      return;
    } else {
      numberOfTries--;
    }
  }
}

void loop() {

  int avg = 0;
  int reading = 0;
  for (int i = 0; i <= 20; i++) {
    avg += WiFi.RSSI();
  }
  reading = avg / 20;

  sensors_event_t event;
  lis.getEvent(&event);
  int xyz = (abs((int)event.acceleration.x) + abs((int)event.acceleration.y) + abs((int)event.acceleration.z)) / 3;
  Serial.println(xyz);
  if (reading < -60) {
    analogWrite(FLICKER, on);
    delay(abs(map(reading, -61, -90, 50, 100)));
    analogWrite(FLICKER, off);
    delay(abs(map(reading, -61, -90, 50, 100)));
  } else if (reading < -30) {
    analogWrite(FLICKER, on);
    delay(abs(map(reading, -30, -60, 10, 50)));
    analogWrite(FLICKER, off);
    delay(abs(map(reading, -30, -60, 10, 50)));
  } else {
    analogWrite(FLICKER, on);
    delay(5);
    analogWrite(FLICKER, off);
    delay(5);
  }
  tone(PIEZO, toneFrequency);  //following is code for the click
  delay(toneLength);
  noTone(PIEZO);
  if ((digitalRead(BUTTON_ONE) == false && CoolDown == false) && FlashNow == false) {  //-- FLASH CODE --//
    i = 255;
    FlashNow = true;
    digitalWrite(FLASH, HIGH);
    digitalWrite(COOL_ONE, LOW);
    digitalWrite(COOL_TWO, LOW);
    digitalWrite(COOL_THREE, LOW);
    digitalWrite(COOL_FOUR, LOW);
    delayMicroseconds(100000);
  }
  if (i > 0 && FlashNow == true) {
    analogWrite(FLASH, i);
    i -= 15;
  }
  if (i == 0 && FlashNow == true) {
    analogWrite(FLASH, i);
    FlashNow = false;
    CoolDown = true;
  }
  if (CoolDown == true) {

    CoolCount++;
    if (CoolCount == (CoolIncrement * 1)) {
      //one light!
      digitalWrite(COOL_ONE, HIGH);
    }
    if (CoolCount == (CoolIncrement * 2)) {
      //two light!
      digitalWrite(COOL_TWO, HIGH);
    }
    if (CoolCount == (CoolIncrement * 3)) {
      //three light!
      digitalWrite(COOL_THREE, HIGH);
    }
    if (CoolCount == (CoolIncrement * 4)) {
      //four light!
      digitalWrite(COOL_FOUR, HIGH);
      CoolDown = false;
      CoolCount = 0;
    }
  }
}

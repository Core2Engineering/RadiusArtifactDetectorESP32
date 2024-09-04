/* Wi-Fi STA Connect and Disconnect Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.

   This is an edited example for grabbing the RSSI of a WiFi access point and using that to blink an LED

*/
#include <WiFi.h>

const char *ssid = "ArtifactDetector";
const char *password = "bruhdontconnecttothis556";

int btnGPIO = 0;
int btnState = false;
int off = 20;
int on = 150;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Set GPIO0 Boot button as input
  pinMode(A5, OUTPUT);
  pinMode(A11, OUTPUT);
  tone(A11, 4000);//following is code for the click
  delay(2);
  noTone(A11);
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

  // Wait for the WiFi event
  while (true) {

    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL: Serial.println("[WiFi] SSID not found"); break;
      case WL_CONNECT_FAILED:
        Serial.print("[WiFi] Failed - WiFi not connected! Reason: ");
        return;
        break;
      case WL_CONNECTION_LOST: Serial.println("[WiFi] Connection was lost"); break;
      case WL_SCAN_COMPLETED:  Serial.println("[WiFi] Scan is completed"); break;
      case WL_DISCONNECTED:    Serial.println("[WiFi] WiFi is disconnected"); break;
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
  //reading = map(reading, -30, -100, 0, 100);
  if (reading < -60){
  analogWrite(A5,on);
  delay(abs(map(reading,-61,-90,50,100)));
  analogWrite(A5,off);
  delay(abs(map(reading,-61,-90,50,100)));
}else if (reading < -30) {
  analogWrite(A5,on);
  delay(abs(map(reading,-30,-60,10,50)));
  analogWrite(A5,off);
  delay(abs(map(reading,-30,-60,10,50)));
}
else{
  analogWrite(A5,on);
  delay(5);
  analogWrite(A5,off);
  delay(5);
}
  tone(A11, 4000);//following is code for the click
  delay(2);
  noTone(A11);
  Serial.println(reading);
  //analogWrite(A5,map(reading,-30,-90,255,0));  //testing, use for led brightness in place of blinking
  //delay(100); 
  
}

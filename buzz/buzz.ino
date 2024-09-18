/*



*/

#include <WiFi.h>
#define btn 5
#define piezo A2
#define flicker A3
#define flash A4
const char *ssid = "ArtifactDetector";
const char *password = "bruhdontconnecttothis556";
int off = 20;
int on = 150;
int toneFrequency = 4000;
int toneLength = 5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);

  // Set GPIO0 Boot button as input
  pinMode(flicker, OUTPUT);
  pinMode(piezo, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
  pinMode(flash,OUTPUT);
  digitalWrite(flash, LOW);

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
  //attachInterrupt(digitalPinToInterrupt(btn), flashLed, RISING);
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
  // put your main code here, to run repeatedly:
  int avg = 0;
  int reading = 0;
  for (int i = 0; i <= 20; i++) {
    avg += WiFi.RSSI();
  }
  reading = avg / 20;
  //Serial.println(reading);
  if (reading < -60){
  analogWrite(flicker,on);
  delay(abs(map(reading,-61,-90,50,100)));
  analogWrite(flicker,off);
  delay(abs(map(reading,-61,-90,50,100)));
}else if (reading < -30) {
  analogWrite(flicker,on);
  delay(abs(map(reading,-30,-60,10,50)));
  analogWrite(flicker,off);
  delay(abs(map(reading,-30,-60,10,50)));
}
else{
  analogWrite(flicker,on);
  delay(5);
  analogWrite(flicker,off);
  delay(5);
}
  tone(piezo, toneFrequency);//following is code for the click
  delay(toneLength);
  noTone(piezo);
  if(digitalRead(btn) == LOW){
          digitalWrite(flash, HIGH);
      delayMicroseconds(100000);
    for(int i = 255; i>= 0; i--){
      analogWrite(flash, i);
      //Serial.println(i);
      delayMicroseconds(3000);
    }
  }
  //Serial.println(reading);
  //analogWrite(flicker,map(reading,-30,-90,255,0));  //testing, use for led brightness in place of blinking
  //delay(100); 
  
}

/*void flashLed() {

}*/

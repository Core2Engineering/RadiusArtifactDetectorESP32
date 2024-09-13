

/* Wi-Fi STA Connect and Disconnect Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.

   This is an edited example for grabbing the RSSI of a WiFi access point and using that to blink an LED

*/
#include <WiFi.h>
#define A8 15
#define btn 33
#define A5 4
#define A11 12
const char *ssid = "ArtifactDetector";
const char *password = "bruhdontconnecttothis556";
int off = 20;
int on = 150;
int flash = 15;
int toneFrequency = 4000;
int toneLength = 5;
TaskHandle_t Task1;
TaskHandle_t Task2;
void Task1code( void *pvParameters ) {
  for(;;){
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
  tone(A11, toneFrequency);//following is code for the click
  delay(toneLength);
  noTone(A11);
  //Serial.println(reading);
  //analogWrite(A5,map(reading,-30,-90,255,0));  //testing, use for led brightness in place of blinking
  //delay(100); 
  
}
}
void Task2code( void *pvParameters ){
  for(;;){
  if (digitalRead(btn) == false){
      digitalWrite(flash, HIGH);
      delay(100);
    for(int i = 255; i>= 0; i--){
      analogWrite(A8, i);
      Serial.println(i);
      delay(3);
    }
  }
}
}
void setup() {
  Serial.begin(115200);
  delay(10);

  // Set GPIO0 Boot button as input
  pinMode(A5, OUTPUT);
  pinMode(A11, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
  pinMode(flash, OUTPUT);
  pinMode(A8,OUTPUT);
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
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(Task1code,"Task1",10000,NULL,1,&Task1,0);           
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
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
}

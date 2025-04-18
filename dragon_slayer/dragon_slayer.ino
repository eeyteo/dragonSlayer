#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>
#include <driver/ledc.h>

// dragon_slayer
// This firmaware is intended to be installed on the
// ESP32-C2 based Electro Dragon WRGB led driver, in order
// for it to be controlled by webhooks

// RGB pin definitions
#define RED_PIN    5
#define GREEN_PIN  6
#define BLUE_PIN   7

// use 12 bit precision
#define LEDC_TIMER_12_BIT 12

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ 5000

const char* ssid = "";
const char* password = "";

// Static IP info
IPAddress local_IP(192, 168, 1, 66);      
IPAddress gateway(192, 168, 1, 1);        
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8,8,8,8);          
IPAddress secondaryDNS(8, 8, 4, 4); //optional

WebServer server(80);  // HTTP on port 80

void ledcAnalogWrite(uint8_t pin, uint32_t value, uint32_t valueMax = 255) {
  // Duty, 4095 from 2 ^ 12 - 1
  uint32_t duty = (4095 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(pin, duty);
}

void setColor(uint32_t red, uint32_t green, uint32_t blue){
  // Write the indicated color
  ledcAnalogWrite(RED_PIN, red);
  ledcAnalogWrite(GREEN_PIN, green);
  ledcAnalogWrite(BLUE_PIN, blue);
}

void handleRoot() {
  server.send(200, "text/plain", "ESP32 is online");
}

void handleRed() {
  // Set the red strip to max
  Serial.println("Activating red strip");
  setColor(255,0,0);
  server.send(200, "text/plain", "RED ON");
}

void handleGreen() {
  // Set the green strip to max
  Serial.println("Activating green strip");
  setColor(0,255,0);
  server.send(200, "text/plain", "GREEN ON");
}

void handleBlue() {
  // Set the blue strip to max
  Serial.println("Activating blue strip");
  setColor(0,0,255);
  server.send(200, "text/plain", "BLUE ON");
}

void handleOff() {
  // Turns off all lines
  setColor(0,0,0);
  server.send(200, "text/plain", "RGB OFF");
}

void handleState() {
  // This returns the state of the 3 lines, via json file
  bool red = (ledcRead(RED_PIN) > 0);
  bool green = (ledcRead(GREEN_PIN) > 0);
  bool blue = (ledcRead(BLUE_PIN) > 0);

  String state = String("{\"red\":") + (red ? "true" : "false") + 
                 ",\"green\":" + (green ? "true" : "false") + 
                 ",\"blue\":" + (blue ? "true" : "false") + "}";
  server.send(200, "application/json", state);
}


void setup() {
  Serial.begin(115200);
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Static IP configuration failed!");
  } else {
    Serial.println("Static IP configured.");
  }
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.print("Connected to static IP:");
  Serial.println(WiFi.localIP());

  // Setup the leds
  ledcAttach(RED_PIN, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcAttach(BLUE_PIN, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcAttach(GREEN_PIN, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);

  // Endpoints
  server.on("/", handleRoot);
  server.on("/red", handleRed);
  server.on("/green", handleGreen);
  server.on("/blue", handleBlue);
  server.on("/off", handleOff);
  server.on("/state", handleState);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

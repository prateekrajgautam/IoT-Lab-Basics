#include <Arduino.h>
#define LED D0
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <LittleFS.h>  // Include the LittleFS library
IPAddress    apIP(42, 42, 42, 42);  // Defining a static IP address: local & gateway
                                    // Default IP in AP mode is 192.168.4.1

/* This are the WiFi access point settings. Update them to your likin */
const char *ssid = "ESP8266_Pass:88888888";
const char *password = "88888888";

// Define a web server at port 80 for HTTP
ESP8266WebServer server(80);

const int ledPin = LED; // an LED is connected to NodeMCU pin D1 (ESP8266 GPIO5) via a 1K Ohm resistor

bool ledState = false;

String readFile(const char* path) {
  File file = LittleFS.open(path, "r");
  if (!file) {
    return String();
  }

  String content;
  while (file.available()) {
    content += file.readStringUntil('\n');
  }
  file.close();

  return content;
}


void handleIndex() {
String html = readFile("/index.html");
  server.send(200, "text/html", html);}

void handleSCSS() {
String css = readFile("/calc.scss");
  server.send(200, "text/css", css);}

void handleJS() {
  String js = readFile("/script.js");
  server.send(200, "application/javascript", js);
}
void handleGeneratorJS() {
  String js = readFile("/generator.js");
  server.send(200, "application/javascript", js);
}

void handleLED() {
  String state = server.arg("state");
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  if (state == "on") {
    digitalWrite(ledPin, HIGH);  // Turn the LED on
  } else if (state == "off") {
    digitalWrite(ledPin, LOW);  // Turn the LED off
  }
  server.send(200, "text/plain", "LED state changed");
}


void setup() {
  LittleFS.begin();  // Initialize the LittleFS file system
  pinMode ( ledPin, OUTPUT );
  digitalWrite ( ledPin, 0 );
	
	delay(1000);
	Serial.begin(115200);
	Serial.println();
	Serial.println("Configuring access point...");

  //set-up the custom IP address
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00  
	
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(ssid, password);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
 
  server.on("/", handleIndex);
  server.on("/index.html", handleIndex);
  server.on("/calc.scss", handleSCSS);
  server.on("/script.js", handleJS);
  server.on("/generator.js", handleGeneratorJS);


  server.on("/led", handleLED);

  server.onNotFound ( handleIndex );
	
	server.begin();
	Serial.println("HTTP server started");
}

void loop() {
	server.handleClient();
}
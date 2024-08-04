/* ----------------------------------------------------------------------------

FOTA_Example.ino : Firmware Over The Air Example

'Firmware Over The Air' allows you to upload sketches remotely.
... No need to connect to your computer when you have an upgrade

  NOTES:

For the ESP8266 install 'ESPAsyncTCP' by dvarrelInstall and for the ESP32 install 'Async TCP' 
by Me-No_Dev (Mathieu Carbou) and 'ESP Async WebServer' by Me-No-Dev via the library manager. 
Note the spaces in the name for 'Async TCP' and 'ESP Async WebServer'

  Builds with :
  - Arduino IDE v2.3.2
  - ESP32 by Espressif Systems v3.0.4
  - ESP8266 by Espressif Systems v3.0.4
  - ESP Async WebServer v3.0.6
  - Async TCP v3.1.4 (for ESP32)
    or
  - AsyncTCP v1.1.4 (for ESP8266)
  For ESP32C3 Super Mini, LOLIN(WEMOS) D1 mini Pro, ...?

  Tested with :
  - ESP32C3 Super Mini
  - ???

Enter the right values in the 'Credentials' section below.

---------------------------------------------------------------------------- */

#ifdef ESP8266
  #include <ESPAsyncTCP.h>
#else
  #include <AsyncTCP.h>
  #include <Update.h>
#endif
#include <ESPAsyncWebServer.h>

// ----- Credentials

String WiFi_ssid = "<Your SSID>";
String WiFi_password = "<Your PWD>";
String FOTA_User = "a";
String FOTA_Password = "a";

// ----- Home page

const char indexhtml[] PROGMEM = R"rawliteral(<!DOCTYPE HTML>
<html lang="en">
<head>
<meta name='viewport' content='width=device-width, height=device-height, initial-scale=1, user-scalable=no'>
<style>
  .rnd_btn {
   background-color: lightgrey;
   border-radius: 50%;
   border-width: 3px;
   border-color: gold;
   color: blue;
   width: 100px;
   height: 50px;
   text-align: center;
  }
</style>
<title>FOTA Example</title>
<body style='background-color: #E6E6FA;'>
<center>
  <h1>FOTA Example</h1>
  Firmware Over The Air<br><br>
  <a href='/FOTA/FOTA'>
   <button class="rnd_btn">FOTA</button>
  </a>
</center>
</body>
</html>
)rawliteral";

// ----- Web page not found

void notFound(AsyncWebServerRequest *request) {
  Serial.println("Client:"
                  + request->client()->remoteIP().toString()
                  + " " + request->url());
  request->send(404, "text/plain", "Not found");
}

// ----- Web Server

AsyncWebServer WebServer(80);

// ----- WiFi

bool initWiFi() {
  Serial.println("\nConnect to WiFi ...");
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.persistent(false);  // do not store WiFi credentials in flash
  WiFi.mode(WIFI_STA);
  WiFi.begin(WiFi_ssid, WiFi_password);
  int count = 0;
  while ((WiFi.status() != WL_CONNECTED) && (count <= 50)) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(200);
    count = count + 1;
  }
  digitalWrite(LED_BUILTIN, 1);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("No WiFi ...");
    return false;
  }
  return true;
}
// ----- Setup

void setup() {

  // Set the Serial port to 115200 baud
  Serial.begin(115200);
  while (! Serial) delay(100);
  Serial.println("\n\nStartup begin...");

  initWiFi();

  Serial.println("\nWebserver Setup...");

  WebServer.onNotFound(notFound);

  WebServer.on("/", HTTP_GET,  [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", indexhtml) ;
  } );

  FOTAConfigureWebServer(WebServer,FOTA_User, FOTA_Password);

  WebServer.begin();

  Serial.println("\nStartup complete...");

  Serial.println("\nBrowse to ... http://"+WiFi.localIP().toString());

}

// ----- Loop

void loop() {
  delay(1000);

}
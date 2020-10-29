
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DallasTemperature.h>
#include <OneWire.h>

const int oneWireBus = 13;           // GPIO where the DS18B20 is connected to
OneWire oneWire(oneWireBus);         // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature sensor

const char* ssid = "Duity&Deepty";
const char* password = "uthinkursmart224455";

const char* serverName = "http://192.168.50.108:5000/"; //Your Domain name with URL path or IP address with path

void setup() {
  Serial.begin(115200);      // Start the Serial Monitor
  sensors.begin();           // Start the DS18B20 sensor

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  String deviceId = String(ESP.getChipId(), HEX);
  Serial.printf(" ESP8266 Chip id = %08X\n", ESP.getChipId());
  Serial.println(" ESP8266 Chip id String Value: " +  deviceId);
  sensors.requestTemperatures();
  
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print("Celsius temperature : ");
  Serial.print(temperatureC);
  Serial.print("ºC");
  Serial.print("  & Farenhite Temperature : ");
  Serial.print(temperatureF);
  Serial.println("ºF");

  if (WiFi.status() == WL_CONNECTED) {
          HTTPClient http;
    
          http.begin(serverName); // Your Domain name with URL path or IP address with path
    
          http.addHeader("Content-Type", "application/json");                                                                                                 // Specify content-type header
          String httpRequestData = "{\"deviceId\":\""+ String(ESP.getChipId(), HEX)
                                    + "\", \"deviceType\":\"Temperature Sensor\", " 
                                    + " \"tempCelsius\":\" " + String(temperatureC) 
                                    + "\", \"tempFarenhite\":\" " + String(temperatureF) 
                                    + "\"}";                                                                                                                 // Data to send with HTTP POST
          Serial.println("Sending Request to ... ");
          int httpResponseCode = http.POST(httpRequestData);                                                                                                 // Send HTTP POST request
    
          String payload = http.getString();
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
    
          http.end(); // Free resources
  } else {
    Serial.println("WiFi Disconnected");
  }
  delay(5000);
}

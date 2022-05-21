#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "x";
const char* password = "x";

// Domain Name with full URL Path for HTTP POST Request
const char* serverName = "http://api.thingspeak.com/update";
// Service API Key
String apiKey = "x";

String plant1name = "Tomato";
String plant2name = "Brinjal";

String apiSend = "api_key=" + apiKey;

// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
// Set timer to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Timer set to 10 seconds (10000)
unsigned long timerDelay = 10000;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");

  // Random seed is a number used to initialize a pseudorandom number generator
  randomSeed(analogRead(33));
}

void loop() {
  float plant1field2 = 0;
  float plant1field3 = 0;
  int plant1field4 = 0;
  float plant2field2 = 0;
  float plant2field3 = 0;
  int plant2field4 = 0;

  plant1field2 = (random(200, 221) / 10.0);
  plant1field3 = (random(60, 73) / 10.0);
  plant1field4 = (random(1, 10));
  plant2field2 = (random(200, 221) / 10.0);
  plant2field3 = (random(60, 73) / 10.0);
  plant2field4 = (random(1, 10));

  //Send an HTTP POST request every 10 seconds
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      // Data to send with HTTP POST
      String httpRequestData1 = apiSend;
      httpRequestData1 += "&field1=";
      httpRequestData1 += "Tomato";
      httpRequestData1 += "&field2=";
      httpRequestData1 += String(plant1field2);
      httpRequestData1 += "&field3=";
      httpRequestData1 += String(plant1field3);
      httpRequestData1 += "&field4=";
      httpRequestData1 += String(plant1field4);
      httpRequestData1 += "&field5=";
      httpRequestData1 += "Brinjal";
      httpRequestData1 += "&field6=";
      httpRequestData1 += String(plant2field2);
      httpRequestData1 += "&field7=";
      httpRequestData1 += String(plant2field3);
      httpRequestData1 += "&field8=";
      httpRequestData1 += String(plant2field4);

      // Send HTTP POST request
      int httpResponseCode1 = http.POST(httpRequestData1);

      Serial.print("HTTP Response code 1: ");
      Serial.println(httpResponseCode1);

      delay(5000);

      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

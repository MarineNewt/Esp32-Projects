#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>  //REQ FOR JSON PARSEING
#include <TFT_eSPI.h>
#include <SPI.h>
#include "secrets.h" // Import API_KEY, WIFI_SSID, and WIFI_PASSWORD

TFT_eSPI tft = TFT_eSPI(); // Create display object

#define BUTTON_A 0     // GPIO for Left button
#define BUTTON_B 35    // GPIO for Right button
#define BACKLIGHT 4    // Optional: control backlight

uint8_t currentColor = 0;

const char* ssid = "Naturae"; 
const char* password = "Moana1524"; //
const char* apiUrlFullEx = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=HOOD&apikey=163K41HVHBRL3B18";
const char* apiUrlFirst = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=";
String apiUrlLast = "&apikey=" + String(API_KEY);
const char* tickers[] = {"HOOD", "GOOG", "TSLA", "AMZN", "AAPL"};
int currentTickerIndex = 0;

unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 200; // ms

//Declare Functions
void updateScreen() {
  switch (currentColor) {
    case 0: tft.fillScreen(TFT_BLACK); break;
    case 1: tft.fillScreen(TFT_RED); break;
    case 2: tft.fillScreen(TFT_GREEN); break;
    case 3: tft.fillScreen(TFT_BLUE); break;
    case 4: tft.fillScreen(TFT_YELLOW); break;
    case 5: tft.fillScreen(TFT_PURPLE); break;
  }
}

void displayTicker() {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.println("Ticker: ");
  tft.println("");
  tft.println(tickers[currentTickerIndex]);
}

//Wifi scan code 
void scanWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();  // Disconnect from any previously connected network
  delay(100);
  int n = WiFi.scanNetworks(); // (No connection needed for scan)
  if (n == 0) {
    tft.println("No networks found.");
  } else {
    tft.printf("Found %d networks:\n\n", n);
    for (int i = 0; i < n && i < 5; i++) {  // Limit to 10 networks for display
      String ssids = WiFi.SSID(i);
      int32_t rssi = WiFi.RSSI(i);
      tft.printf("%d: %s (%ddBm)\n", i + 1, ssids.c_str(), rssi);
    }
  }
}

//////////////////////////////////////////////////////////////Primary Run Code
void setup() {
  Serial.begin(921600);
  Serial.println("Hello World");

  tft.init();
  tft.setRotation(0);  // 0 for Vert, 1 for Landscape mode
  tft.fillScreen(TFT_BLACK);

  // Enable backlight
  pinMode(BACKLIGHT, OUTPUT);
  digitalWrite(BACKLIGHT, HIGH);

  // Setup buttons
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT); // GPIO 35 has no pull-up/down — use external resistor or detect active-low

  // Setup Text
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.println("Scanning WiFi...");
  tft.println("Connecting to:");
  tft.println(WIFI_SSID);

  // Start WiFi 
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();  // Disconnect from any previously connected network
  delay(100);
  // int n = WiFi.scanNetworks(); // (No connection needed for scan)
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //Connect to Network 

  int dotCount = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tft.print(".");
    dotCount++;
    if (dotCount > 25) {
      tft.println("\nTimeout!");
      return;
    }
  }

  //Connect success confirmation.
  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.println("Connected!");
  //tft.print("IP: ");
  //tft.println(WiFi.localIP());
  
  delay(3000);
  displayTicker();
}

void loop() {
  // Debounce timing
  unsigned long currentMillis = millis();
  if (currentMillis - lastButtonPress < debounceDelay) return;
  
  // Read buttons (active low)
  bool pressedA = digitalRead(BUTTON_A) == LOW;
  bool pressedB = digitalRead(BUTTON_B) == LOW;

  if (pressedA) {
    currentTickerIndex = (currentTickerIndex + 1) % 5;
    Serial.println("Next: " + String(tickers[currentTickerIndex]));
    displayTicker();
    lastButtonPress = currentMillis;
  }
  
  if (pressedB) {
    //Display notice of event taking place
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.println("Analyzing ");
    tft.println(tickers[currentTickerIndex]);

    // Make HTTP GET request
    char apiUrl[200];
    snprintf(apiUrl, sizeof(apiUrl), "%s%s%s", apiUrlFirst, tickers[currentTickerIndex], apiUrlLast);
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(apiUrl);

      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        String payload = http.getString();
        Serial.println(payload);

        // Parse JSON (optional)
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        const JsonObject quote = doc["Global Quote"];

        const char* ticker = quote["01. symbol"];
        const char* price = quote["05. price"];

        // Display result
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.println("Ticker:"); //tft.println takes only 1 argument and does not support inline variables. 
        tft.println(ticker); //Variables must be split and printed individually like this.
        tft.println("Price:");
        tft.println(price);
      } else {
        Serial.println("Error in HTTP request");
        tft.println("HTTP Request Failed");
      }
      http.end();
    }
    lastButtonPress = currentMillis;
  }
  delay(20);  // Small delay to reduce cpu usage
}

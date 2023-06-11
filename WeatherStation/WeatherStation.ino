#include <TFT_eSPI.h>

#include <DHTesp.h> 
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "time.h"

#include <PNGdec.h>
PNG png; // PNG decoder instance
#define MAX_IMAGE_WDITH 240 // Adjust for your images
int16_t xpos = 0;
int16_t ypos = 0;

/** Initialize DHT sensor */
DHTesp dht;
/** Task handle for the light value read task */
TaskHandle_t tempTaskHandle = NULL;
/** Pin number for DHT11 data pin */
int dhtPin = 2;

#include <TFT_eSPI.h> // Hardware-specific library
int screenVerticalSize = 240;
int screenHorizontalSize = 320;
#include <SPI.h>

#include "Settings.h"
#include "weathericons.h"
const String apiUrl = "https://api.meteo-concept.com/api/forecast/daily?world=true&start=0&end=3";
TFT_eSPI tft = TFT_eSPI();    

WiFiClientSecure client;

const char* ca_cert = "-----BEGIN CERTIFICATE-----\n" \
"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\n" \
"-----END CERTIFICATE-----";


void clearScreen(){
  tft.fillScreen(TFT_BLACK);
}

void writeCenterMessage(String message){
  clearScreen();
  int textSize = 2;
  tft.setCursor(0, (screenVerticalSize/2)-(textSize*10/2));
  tft.setTextColor(TFT_WHITE, TFT_BLACK); 
  tft.setTextSize(textSize);
  tft.println(message);
}

void connectWifi() {

  writeCenterMessage((String)"Connecting to Wifi : " + WIFI_SSID);
  WiFi.mode(WIFI_STA); 

  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(1000);
  }
  Serial.println();
}

void writeTemperatureAndHumidity(){   
    tft.setCursor(0, 20);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); 
    tft.setTextSize(20);
    if (dht.getStatus() != 0) {
      tft.println("temperature sensor error status: " + String(dht.getStatusString()));
      return;
    }
  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
    TempAndHumidity lastValues = dht.getTempAndHumidity();
    String temperature = String(lastValues.temperature,0)+"C";
    String humidity = String(lastValues.humidity,0) + "%";
    tft.println(temperature);
    tft.println(humidity);
}

DynamicJsonDocument getWeatherFromApi(String url) {
  // use https://arduinojson.org/v6/assistant to get doc size
  DynamicJsonDocument doc(8192);
  HTTPClient http;

  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.GET();
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    DeserializationError error = deserializeJson(doc, http.getString());
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
    }
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return doc;
}

void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void drawPng(unsigned char* pngImage, int pngSize,int16_t pngXpos, int16_t pngYpos) {
  ypos = pngYpos;
  xpos = pngXpos;
  int16_t rc = png.openFLASH((uint8_t *)pngImage, pngSize, pngDraw);
  if (rc == PNG_SUCCESS) {
    tft.startWrite();
    uint32_t dt = millis();
    rc = png.decode(NULL, 0);
    tft.endWrite();
  }
  else{
    Serial.println("Failed png file. Returned code : " + (String)rc);    
  }
}

const char * months[] = { 
    "Janvier", "Février", "Mars", "Avril", "Mai", "Juin", "Juillet",
    "Août", "Septembre", "Octobre", "Novembre", "Décembre"
};
const char * days[] = {
    "Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"
};

String formatDate(const char* datetime, int nameLength = 10){
  struct tm ts = {0};
  strptime(datetime,"%Y-%m-%dT%H%M%S%Z",&ts);
  mktime(&ts);
  return String(days[ts.tm_wday]).substring(0, nameLength) + " " +  String(ts.tm_mday) + " " +  String(months[ts.tm_mon]).substring(0, nameLength);
}

void drawForecast(JsonVariant forecast, int16_t iconpos){
  drawPng(getWeatherIcon(forecast["weather"].as<int>()), getWeatherIconSize(forecast["weather"].as<int>()), iconpos, 130);
  Serial.println(formatDate(forecast["datetime"].as<const char*>(), 3));
  tft.setCursor(iconpos, 120);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); 
  tft.setTextSize(1);
  tft.println(formatDate(forecast["datetime"].as<const char*>(), 3));
}

void drawTodayWeather(JsonVariant forecast){
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK); 
  tft.setTextSize(2);
  drawPng(getWeatherIcon(forecast["weather"].as<int>()), getWeatherIconSize(forecast["weather"].as<int>()), 0, 60);
  Serial.println(formatDate(forecast["datetime"].as<const char*>()));
  tft.setCursor(0, 5);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); 
  tft.setTextSize(2);
  tft.println(formatDate(forecast["datetime"].as<const char*>(), 3));
}



/*
 * Initialize the serial port.
 */
void setup( )
{
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(1);
  clearScreen();
  dht.setup(dhtPin, DHTesp::DHT11);
  client.setCACert(ca_cert);
}
/*
 * Main program loop.
 */
void loop( )
{
  clearScreen();
  // writeTemperatureAndHumidity();
  // connect to WiFi 
  if (WiFi.status() != WL_CONNECTED) {
    connectWifi();
  }
  if(WiFi.status() == WL_CONNECTED){
    String url = apiUrl + "&token=" + METEO_TOKEN + "&latlng=" + METEO_LATITUDE + "," + METEO_LONGITUDE;
    Serial.println(url);
    DynamicJsonDocument doc = getWeatherFromApi(url);
    JsonArray forecastArray = doc["forecast"].as<JsonArray>();
    clearScreen();
    for(int i = 0; i < forecastArray.size(); i++) {
      JsonVariant forecast = forecastArray[i];
      if(i == 0){
        drawTodayWeather(forecast);
      }
      else{
        drawForecast(forecast, 120*(i-1));
      }
    }
  }
  delay(3600000); // 1h
}

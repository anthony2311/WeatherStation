#include <DHTesp.h>
#include <DHTesp.h> 

/** Initialize DHT sensor */
DHTesp dht;
/** Task handle for the light value read task */
TaskHandle_t tempTaskHandle = NULL;
/** Pin number for DHT11 data pin */
int dhtPin = 2;

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();    

/*
 * Initialize the serial port.
 */
void setup( )
{
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  dht.setup(dhtPin, DHTesp::DHT11);
}
/*
 * Main program loop.
 */
void loop( )
{
  delay(10000);
  // clear screen
  tft.fillScreen(TFT_BLACK);
  writeTemperatureAndHumidity();
  
}

void writeTemperatureAndHumidity(){   
    tft.setCursor(0, 5);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); 
    tft.setTextSize(10);
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

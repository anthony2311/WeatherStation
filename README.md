# WeatherStation
Weather station 


# Setup

## Hardware

- [ESP32](https://www.az-delivery.de/en/products/esp32-developmentboard)
- [DHT11](https://www.az-delivery.de/en/products/5-x-dht11-temperatursensor)
- [ILI9341](https://www.az-delivery.de/en/products/2-4-tft-lcd-touch-display) 2.4 inch TFT LCD Touch Display TFT touch screen 

## ESP32 board installation

In your Arduino IDE, go to **File** > **Preferences**

Enter the following into the **Additional Board Manager URLs** field:

``https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json``

Open the Boards Manager. Go to **Tools** > **Board** > **Boards Manager…**

Search and install the following packages :
 - **ESP32** by **Espressif Systems** (2.0.5)

## Libraries installation

Open the libraries manager. Go to **Sketch** > **Include Library** > **Manage Libraries**

Search and install the following packages : 

 - **DHT sensor library for ESPx** by **beegee_tokyo** (1.18.0)
 - **TFT_eSPI** by **Bodmer** (2.4.79)
 - **ArduinoJson** by **Benoit Blanchon** (6.19.4)

## Libraries configuration

Now you have the library installed you have to configure the IO pins where we have connected our screen. Other libraries use the definition of constants at the top of each sketch to do this, but TFT_eSPI uses a common file to define the configuration.

Find in your Arduino installation the file called User_Setup.h

It should be located in a path equivalent to this:

``[Arduino Folder]/libraries/TFT_eSPI/User_Setup.h``

Now, copy the file for backup purposes and edit the original to place inside it this following content (replacing all the previous content):

```
#define ESP32_PARALLEL


#define ILI9341_DRIVER


// ESP32 pins used for the parallel interface TFT
#define TFT_CS   27  // Chip select control pin
#define TFT_DC   14  // Data Command control pin - must use a pin in the range 0-31
#define TFT_RST  26  // Reset pin

#define TFT_WR   12  // Write strobe control pin - must use a pin in the range 0-31
#define TFT_RD   13

#define TFT_D0   16  // Must use pins in the range 0-31 for the data bus
#define TFT_D1   4  // so a single register write sets/clears all bits
#define TFT_D2   23
#define TFT_D3   22
#define TFT_D4   21
#define TFT_D5   19
#define TFT_D6   18
#define TFT_D7   17


#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT
```
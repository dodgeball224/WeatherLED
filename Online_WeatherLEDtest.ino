#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> // http web access library
#include <ArduinoJson.h> // JSON decoding library
// Libraries for SSD1306 OLED display
#include <Wire.h> // include wire library (for I2C devices such as the SSD1306 display)
#include <Adafruit_GFX.h> // include Adafruit graphics library
#include <Adafruit_SSD1306.h> // include Adafruit SSD1306 OLED display driver
#include <FastLED.h>
#define LED_PIN  1
#define NUM_LEDS 5
CRGB leds[NUM_LEDS];

#define OLED_RESET 5 // define SSD1306 OLED reset at ESP8266 GPIO5 (NodeMCU D1)
Adafruit_SSD1306 display(OLED_RESET);

// set Wi-Fi SSID and password
const char *ssid = "fakewifi";
const char *password = "fakepassword";

// set location and API key
String Location = "Seattle, US";
String API_Key = "fakeAPIkey";

void setup(void)
{
Serial.begin(9600);
delay(1000);

Wire.begin(4, 0); // set I2C pins [SDA = GPIO4 (D2), SCL = GPIO0 (D3)], default clock is 100kHz

// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
// init done

Wire.setClock(400000L); // set I2C clock to 400kHz

display.clearDisplay(); // clear the display buffer
display.setTextColor(WHITE, BLACK);
display.setTextSize(1);
display.setCursor(0, 0);
display.println(" Internet Weather");
display.print(" Seattle WA");
display.display();

WiFi.begin(ssid, password);

Serial.print("Connecting.");
display.setCursor(0, 24);
display.println("Connecting...");
display.display();
while ( WiFi.status() != WL_CONNECTED )
{
delay(500);
Serial.print(".");
}
Serial.println("connected");
display.print("connected");
display.display();
delay(1000);

FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

}

void loop()
{
if (WiFi.status() == WL_CONNECTED) //Check WiFi connection status
{
HTTPClient http; //Declare an object of class HTTPClient

// specify request destination
http.begin("http://api.openweathermap.org/data/2.5/weather?q=" + Location + "&APPID=" + API_Key); // !!

int httpCode = http.GET(); // send the request

if (httpCode > 0) // check the returning code
{
String payload = http.getString(); //Get the request response payload

DynamicJsonBuffer jsonBuffer(512);

// Parse JSON object
JsonObject& root = jsonBuffer.parseObject(payload);
if (!root.success()) {
Serial.println(F("Parsing failed!"));
return;
}

float temp = (float)(root["main"]["temp"]) - 273.15; // get temperature in °C
int humidity = root["main"]["humidity"]; // get humidity in %
float pressure = (float)(root["main"]["pressure"]) / 1000; // get pressure in bar
float wind_speed = root["wind"]["speed"]; // get wind speed in m/s
int wind_degree = root["wind"]["deg"]; // get wind degree in °


    // Need statement for if weather is cloudy turn these LEDS on
      for (int i = 0; i <= 4; i++) {
        leds[i] = CRGB ( 150, 150, 150);
        FastLED.show();
        delay(400);
      }
      for (int i = 4; i >= 0; i--) {
      leds[i] = CRGB ( 0, 0, 150);
      FastLED.show();
      delay(400);
      }
      }
  // Need statement for if clear turn these LEDS on
      leds[0] = CRGB(0, 0, 255);
      FastLED.show();
      delay(500);  
      leds[1] = CRGB(255, 255, 51);
      FastLED.show();
      delay(500);
      leds[2] = CRGB(0, 0, 255);
      FastLED.show();
      delay(500);
      leds[3] = CRGB(255, 255, 51);
      FastLED.show();
      delay(500);
      leds[4] = CRGB(0, 0, 255);
      FastLED.show();
      delay(500);
  }

// print data
//Serial.println("Main = %.2f\r\n", main);
Serial.printf("Temperature = %.2f°C\r\n", temp);
Serial.printf("Humidity = %d %%\r\n", humidity);
Serial.printf("Pressure = %.3f bar\r\n", pressure);
Serial.printf("Wind speed = %.1f m/s\r\n", wind_speed);
Serial.printf("Wind degree = %d°\r\n\r\n", wind_degree);

display.setCursor(0, 24);
//display.printf("Main = %.2f\r\n", main); 
display.printf("Temperature: %5.2f C\r\n", temp);
display.printf("Humidity : %d %%\r\n", humidity);
display.printf("Pressure : %.3fbar\r\n", pressure);
display.printf("Wind speed : %.1f m/s\r\n", wind_speed);
display.printf("Wind degree: %d", wind_degree);
display.drawRect(109, 24, 3, 3, WHITE); // put degree symbol ( ° )
display.drawRect(97, 56, 3, 3, WHITE);



}

http.end(); //Close connection

}

delay(60000); // wait 1 minute

}
// End of code.


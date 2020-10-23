#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

String readBT="";

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(300);

  Serial.begin(9600); //for BLE module

  display.clearDisplay();
  display.display();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Online!\nv1.0");
  display.display(); // actually display all of the above

  delay(300);
  display.clearDisplay();
  display.display();

  Serial.println("AT");
}

void loop()
{
  if(Serial.available())
  {
    readBT=Serial.readString();
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(readBT);
    display.display();
  }
}

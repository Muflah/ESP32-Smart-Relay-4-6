#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

void setup() {
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  //display.display();
  //delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  // text display tests
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Online!\nSTM32F103C8");
  //display.setCursor(0,0);
  display.display(); // actually display all of the above

  delay(500);
  display.clearDisplay();
  display.display();
}

void loop()
{
  //Serial.println("AT");
  //Serial.println("AT + RESET");
  if(Serial.available())
  {
    String xyz=Serial.readString();
    display.setCursor(0, 0);
    display.print(xyz);
    display.display();
    delay(500);
    display.setCursor(0, 0);
    display.clearDisplay();
    display.display();
  }
  delay(300);
}

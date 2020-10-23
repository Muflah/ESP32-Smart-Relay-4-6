#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AltSoftSerial.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

AltSoftSerial altSerial;

String readBT="";

const int psw=PD2;
const int sw[6]={10, A0, A1, A2, A3, 3};
const int inp[4]={4, 5, 6, 7};

void setup()
{
  altSerial.begin(9600);
  delay(1000);
  altSerial.println("Hello World");

  pinMode(psw, OUTPUT);
  digitalWrite(psw, LOW);

  for(int i=0; i<6; i++)
  {
    pinMode(sw[i], OUTPUT);
    digitalWrite(sw[i], LOW);
  }

  for(int i=0; i<4; i++)
  {
    pinMode(inp[i], INPUT);
  }

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

bool blink=false;
void loop()
{
  delay(1000);

  blink=!blink;
  digitalWrite(psw, blink);
  for(int i=0; i<6; i++)
  {
    digitalWrite(sw[i], blink);
  }

  for(int i=0; i<4; i++)
  {
    altSerial.println("IN" + String(i) + ": " + String(digitalRead(inp[i])));
    Serial.println("IN" + String(i) + ": " + String(digitalRead(inp[i])));
  }

  if(Serial.available())
  {
    readBT=Serial.readString();
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(readBT);
    display.display();
  }
  if(altSerial.available())
  {
    readBT=altSerial.readString();
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(readBT);
    display.display();
  }
}

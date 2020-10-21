#include <WiFi.h>

#define U1TX 2
#define U1RX 4

String defaultSSID="fallback", defaultPWD="fallback";

const int relay[6]={12, 13, 14, 25, 26, 27};
const int pwrRelay=15;
const int inp[4]={32, 33, 34, 35};
const int buzzer=5;

bool sysStatus=false;
bool WiFiPriority=true;

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("");
  Serial.println("");
  Serial.println("Online!");
  Serial.println("v1.2");
  Serial.println("MAC: " + String(WiFi.macAddress()));

  Serial1.begin(9600, SERIAL_8N1, U1RX, U1TX);

  for(int i=0; i<6; i++)
  {
    pinMode(relay[i], OUTPUT);
    digitalWrite(relay[i], LOW);
  }

  pinMode(pwrRelay, OUTPUT);
  digitalWrite(pwrRelay, LOW);

  for(int i=0; i<4; i++)
  {
    pinMode(inp[i], INPUT);
  }

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  if(!initSys())
  {
    Serial.println("System initialization failed!");
  }
  else
  {
    Serial.println("System booted sucessfully.");
  }
}

bool blink=false;
void loop()
{
  if(!sysStatus)
  {
    runtimeFailSafe();
  }
  if(WiFi.status() != WL_CONNECTED && WiFiPriority)
  {
    Serial.println("WiFi lost or not found.");
    sysStatus=connectWiFi();
  }
  else
  {
    sysStatus=true;
  }
  blink=!blink;
  for(int i=0; i<4; i++)
  {
    if(digitalRead(inp[i]))
    {
      Serial.println("Input on inp" + String(i) +".");
    }
  }
  for(int i=0; i<6; i++)
  {
    digitalWrite(relay[i], blink);
  }
  digitalWrite(pwrRelay, blink);
  digitalWrite(buzzer, blink);
  delay(200);

  if(Serial1.available())
  {
    String abc=Serial1.readString();
    Serial.println(abc);
  }
  if(Serial.available())
  {
    String abc=Serial.readString();
    Serial1.println(abc);
  }
}

bool initSys()
{
  loadParameters();
  for(int i=0; i<3 && !sysStatus; i++)
  {
    sysStatus=connectWiFi();
    delay(500);
  }

  if(sysStatus==false)
  {
    failSafe();
  }

  return sysStatus;
}

void loadParameters()
{
  //read values from ROM
}

bool connectWiFi()
{
  WiFi.begin("DESKTOP-U6C56CM 3446", "abcd1234");
  int attempt=0;

  Serial.println("");

  Serial.print("Establishing WiFi connection");

  while (WiFi.status() != WL_CONNECTED && attempt<3)
  {
    attempt++;
    delay(300);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("");

  if(attempt<3)
  {
    Serial.println("WiFi connected!");
    return 1;
  }
  else
  {
    Serial.println("WiFi not connected!");
    return 0;
  }
}

void failSafe()
{
  Serial.println("");
  Serial.println("Attempting failsafe...");
  Serial.println("");
}

void runtimeFailSafe()
{
  Serial.println("");
  Serial.println("Attempting runtime failsafe...");
  Serial.println("");
}

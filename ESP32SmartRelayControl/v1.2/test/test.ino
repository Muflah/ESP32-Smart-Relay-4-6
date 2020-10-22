#include <WiFi.h>

#define U1TX 2
#define U1RX 4

const char* defaultSSID="fallback";
const char* defaultPWD="fallback";

char* ssid="fallback";
char* pwd="fallback";

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

  //Serial1.begin(9600, SERIAL_8N1, U1RX, U1TX);

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
  sysStatus=checkSysStatus();

  Serial.println("System status: "+ String(sysStatus));

  if(!sysStatus)
  {
    runtimeFailSafe();
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
    String s1dat=Serial1.readString();
    Serial.println(s1dat);
  }
  if(Serial.available())
  {
    String s0dat=Serial.readString();
    Serial1.println(s0dat);
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

  ssid="abc";
  pwd="pwd";
}

bool connectWiFi()
{
  WiFi.begin(ssid, pwd);
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

bool checkSysStatus()
{
  bool sysStatus=false;
  if(WiFi.status() != WL_CONNECTED && WiFiPriority)
  {
    Serial.println("WiFi lost or not found.");
    Serial.print("Searching for default network");

    for(int i=0; i<3; i++)
    {
      int attempt=0;
      WiFi.begin(defaultSSID, defaultPWD);
      while (WiFi.status() != WL_CONNECTED && attempt<3)
      {
        attempt++;
        delay(300);
        Serial.print(".");
      }
    }
    Serial.println("");
  }

  if(WiFi.status() != WL_CONNECTED && WiFiPriority)
  {
    sysStatus=false;
  }
  else
  {
    sysStatus=true;
  }
  return sysStatus;
}

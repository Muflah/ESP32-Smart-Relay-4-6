#include <WiFi.h>

#define U1TX 2
#define U1RX 4

const char* defaultSSID="fallback";
const char* defaultPWD="fallback";

char* ssid="";
char* pwd="";

const int relay[6]={12, 13, 14, 25, 26, 27};
const int pwrRelay=15;
const int inp[4]={32, 33, 34, 35};
const int buzzer=5;

bool relayStatus[6]={0, 0, 0, 0, 0, 0};
bool pwrRelayStatus=0;

bool sysStatus=false;
bool WiFiPriority=true;

void setup()
{
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

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

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  delay(100);
  Serial.println("");
  Serial.println("");
  Serial.println("Online!");
  Serial.println("v1.2");
  Serial.println("MAC: " + String(WiFi.macAddress()));

  Serial1.begin(9600, SERIAL_8N1, U1RX, U1TX);

  if(!initSys())
  {
    Serial.println("System initialization failed!");
  }
  else
  {
    Serial.println("System booted sucessfully.");
  }
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
}

void loop()
{
  sysStatus=checkSysStatus();

  Serial.println("\n\nSystem status: "+ String(sysStatus) +"\n");

  if(!sysStatus)
  {
    failSafe();
  }

  for(int i=0; i<4; i++)
  {
    Serial.println("IN" + String(i)+ ": "+ String(digitalRead(inp[i])));
  }

  for(int i=0; i<6; i++)
  {
    digitalWrite(relay[i], relayStatus[i]);
    Serial.println("OUT"+ String(i)+ ": "+ String(relayStatus[i]));
  }

  digitalWrite(pwrRelay, pwrRelayStatus);
  Serial.println("PR: "+ String(pwrRelayStatus));

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
  //ideally read values from ROM

  ssid="fallback";
  pwd="fallback";
  WiFiPriority=true;
  relayStatus[0]=0;
  relayStatus[1]=0;
  relayStatus[2]=0;
  relayStatus[3]=0;
  relayStatus[4]=0;
  relayStatus[5]=0;
  pwrRelayStatus=0;
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
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  Serial.println("");
  Serial.println("Attempting failsafe...");
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

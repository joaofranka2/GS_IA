#include <ThingerESP32.h>

#define USERNAME "Jonas_GS"
#define DEVICE_ID "GS_IA"
#define DEVICE_CREDENTIAL "123456"

#define SSID "Wokwi-GUEST"
#define SSID_PASSWORD ""
#define PIN_AMARELO 33

#define teste "Ola mundo"

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
void setup()
{

  pinMode(PIN_AMARELO, OUTPUT);

  thing.add_wifi(SSID, SSID_PASSWORD);
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  thing["Led Amarelo"] << digitalPin(33);
  thing["teste"] >> outputValue(teste);
}

void loop()
{
  // put your main code here, to run repeatedly:
  delay(2000);
  thing.handle();
}
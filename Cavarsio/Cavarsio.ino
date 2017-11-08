#include <ESP8266WiFi.h>

void setup(void)
{
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Wi-Fi mode set to WIFI_STA %s\n", WiFi.mode(WIFI_STA) ? "" : "Failed!");
  Serial.print("Begin WPS (press WPS button on your router) ... ");
  Serial.println(WiFi.beginWPSConfig() ? "Success" : "Failed");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {}

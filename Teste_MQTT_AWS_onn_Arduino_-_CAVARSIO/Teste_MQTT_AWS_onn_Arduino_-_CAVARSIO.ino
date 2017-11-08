/*
 Basic Amazon AWS IoT example
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiSSLClient.h>
#include <MQTT.h>

// Update these with values suitable for your network.
char ssid[] = "My ASUS";     // your network SSID (name)
char pass[] = "saladastreta";  // your network password
int status  = WL_IDLE_STATUS;    // the Wifi radio's status

WiFiSSLClient wifiClient;

PubSubClient client(wifiClient);

#define THING_NAME "CavarsIoT"

char mqttServer[]           = "a3spyhhv84qh1v.iot.us-east-1.amazonaws.com";
char clientId[]             = "mqtt-client-arduino-1";
char topic_publish = "sync_arduino";
char topic_subscribe = "subs_arduino";
char publishPayload[MQTT_MAX_PACKET_SIZE];


/* root CA can be download here:
 *  https://www.symantec.com/content/en/us/enterprise/verisign/roots/VeriSign-Class%203-Public-Primary-Certification-Authority-G5.pem
 **/
char* rootCABuff = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\n" \
"yjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQL\n" \
"ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\n" \
"U2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxW\n" \
"ZXJpU2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0\n" \
"aG9yaXR5IC0gRzUwHhcNMDYxMTA4MDAwMDAwWhcNMzYwNzE2MjM1OTU5WjCByjEL\n" \
"MAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZW\n" \
"ZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJpU2ln\n" \
"biwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxWZXJp\n" \
"U2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9y\n" \
"aXR5IC0gRzUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCvJAgIKXo1\n" \
"nmAMqudLO07cfLw8RRy7K+D+KQL5VwijZIUVJ/XxrcgxiV0i6CqqpkKzj/i5Vbex\n" \
"t0uz/o9+B1fs70PbZmIVYc9gDaTY3vjgw2IIPVQT60nKWVSFJuUrjxuf6/WhkcIz\n" \
"SdhDY2pSS9KP6HBRTdGJaXvHcPaz3BJ023tdS1bTlr8Vd6Gw9KIl8q8ckmcY5fQG\n" \
"BO+QueQA5N06tRn/Arr0PO7gi+s3i+z016zy9vA9r911kTMZHRxAy3QkGSGT2RT+\n" \
"rCpSx4/VBEnkjWNHiDxpg8v+R70rfk/Fla4OndTRQ8Bnc+MUCH7lP59zuDMKz10/\n" \
"NIeWiu5T6CUVAgMBAAGjgbIwga8wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8E\n" \
"BAMCAQYwbQYIKwYBBQUHAQwEYTBfoV2gWzBZMFcwVRYJaW1hZ2UvZ2lmMCEwHzAH\n" \
"BgUrDgMCGgQUj+XTGoasjY5rw8+AatRIGCx7GS4wJRYjaHR0cDovL2xvZ28udmVy\n" \
"aXNpZ24uY29tL3ZzbG9nby5naWYwHQYDVR0OBBYEFH/TZafC3ey78DAJ80M5+gKv\n" \
"MzEzMA0GCSqGSIb3DQEBBQUAA4IBAQCTJEowX2LP2BqYLz3q3JktvXf2pXkiOOzE\n" \
"p6B4Eq1iDkVwZMXnl2YtmAl+X6/WzChl8gGqCBpH3vn5fJJaCGkgDdk+bW48DW7Y\n" \
"5gaRQBi5+MHt39tBquCWIMnNZBU4gcmU7qKEKQsTb47bDN0lAtukixlE0kF6BWlK\n" \
"WE9gyn6CagsCqiUXObXbf+eEZSqVir2G3l6BFoMtEMze/aiCKm0oHw0LxOXnGiYZ\n" \
"4fQRbxC1lfznQgUy286dUV4otp6F01vvpX1FQHKOtw5rDgb7MzVIcbidJ4vEZV8N\n" \
"hnacRHr2lVz2XTIIM6RUthg/aFzyQkqFOFSDX9HoLPKsEdao7WNq\n" \
"-----END CERTIFICATE-----\n" \

/* Fill your certificate.pem.crt wiht LINE ENDING */
char* certificateBuff = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDWjCCAkKgAwIBAgIVAIGmMlzFIyRV5AoK8JeE8j6Wvd2hMA0GCSqGSIb3DQEB\n" \
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n" \
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xNzEwMjIxNTU4\n" \
"MTdaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n" \
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCiF04okyGfEkTVdNTb\n" \
"NoFhbXwHVI4+hkx7FQTLWjxce163NuOaQTLBQTt/rnGi2DXr0jqhxVTZgFNR4UKn\n" \
"jeHz0rakOXcKVSx2B+DaIF7ngstLHIrXnxCjY/Y11Gf33kd+b9r6lfDYk2pM1K6B\n" \
"opY8hYab9s6UIaDRHMt0O4hhTae1fVOndsrIqdF/HgzbXHKgC2JFNYLpx36s3uc8\n" \
"4voxJPi+AzepT+hqdsdgD1w+ylQWsy/NtoGGKDH5gaIDhxnBagjVt3tsn1LnnkUq\n" \
"9jVo+/b+TI4bsm9DpyUKFXA5l4sNc8dY0z9x/c3uP0MpIXr3lXYPzyu89NDsFyPv\n" \
"UUj/AgMBAAGjYDBeMB8GA1UdIwQYMBaAFKpC6sgOdwwrijY13EC9Ia3MirCiMB0G\n" \
"A1UdDgQWBBRVq4n21J73zleJpxpm3PGYjGRRVjAMBgNVHRMBAf8EAjAAMA4GA1Ud\n" \
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAReqSZkrQDCOhu0coIXJWyj1L\n" \
"PsTMxmWTjvxsfbCGBLmpUgYmJHYEyiD00c/iTg5mIzk9SmXdfPFbbzdhTP3O9Qbc\n" \
"wyCgLrtTOjs8pcynUghOTbRqRgxWFpbs1abvKMulEZNxTOL8GQ04g1f9TCfYBYBf\n" \
"o3JwDk9pCBxda0hlPUU/SMSFiy15rtn8PHVR7pcZe6QCanCxmaexq+cI8L9EnR0a\n" \
"mF01jCWgHDBEWXgkiviTpsdBNkyoUVays4Quy01dMuPd1c/Dko1hENQTR8J6vmSZ\n" \
"jDdTX2DlEbJuJGp/PWbWtALl0vHfgaSwVrje0XS/JNOJsFsM2onDhMpZo0rcIw==\n" \
"-----END CERTIFICATE-----"; 

/* Fill your private.pem.key wiht LINE ENDING */
char* privateKeyBuff = \
"-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEogIBAAKCAQEAohdOKJMhnxJE1XTU2zaBYW18B1SOPoZMexUEy1o8XHtetzbj\n" \
"mkEywUE7f65xotg169I6ocVU2YBTUeFCp43h89K2pDl3ClUsdgfg2iBe54LLSxyK\n" \
"158Qo2P2NdRn995Hfm/a+pXw2JNqTNSugaKWPIWGm/bOlCGg0RzLdDuIYU2ntX1T\n" \
"p3bKyKnRfx4M21xyoAtiRTWC6cd+rN7nPOL6MST4vgM3qU/oanbHYA9cPspUFrMv\n" \
"zbaBhigx+YGiA4cZwWoI1bd7bJ9S555FKvY1aPv2/kyOG7JvQ6clChVwOZeLDXPH\n" \
"WNM/cf3N7j9DKSF695V2D88rvPTQ7Bcj71FI/wIDAQABAoIBAGP2/4rigp+6Ap2q\n" \
"egPf75cIkK9spHVHGi/mXJiVRBi9NULFJUVi5ExvUKSEDy0hLtALTYGrU5L4I2qi\n" \
"BBuV0z4uO9m4XwQOV2dLoI6l3CHndj0Sk0Y7oWVDJvs3zg7+FRWTInfctTa8Y5iS\n" \
"qjTk8sbMiqHxaqHMw4NzwL4kYQ2weDzhsjnZCKLGaPvSgAeQsCNuen7Hs+Qefv6e\n" \
"KZSKk7HqKat1i+n0s8kQJP45TAx7cGaR7eAnqK80XKzN2mMpJvZNF9eniq/sWEnK\n" \
"3gDhCpNM+pOQWjfN/MmgGRzzKIGdYCHly06bql8rOrGna6RzZK9CyxrpYx0e4/pG\n" \
"bdEVUzECgYEA+axt4gCDENEzHz05qJnJ8nagHKvphVQT9iIln6YJeWmTqh0sEwx4\n" \
"Knr4hPdj6vekNVkNLIsZgXLjmCzgkJsZoCVdjFpLjaQsk8ykPDly2MiRqa59DPGa\n" \
"+18Mnf6GdPyXC/Qq7POVu6uLE+bptFVdF5HX5lpnZ63VAOXLMxCDCksCgYEApjK/\n" \
"+RJqi7Fq8ToHYWsN31u0oWsfQcaVTSYl7bh+a9y5MzsJIiiGVCmP4TU2Q+dlyC7R\n" \
"pl400QJzj3fD8nRZzSsuDfcMgGBJ2qTYAtBa99299bL0RHKP0RQWxFMQxv37M/jB\n" \
"LNvxQxoIGqoTDq6utO9NLz8cqMuOHYSKI2rQS50CgYAGkV6in1jTTG2H2Nh3cvQ5\n" \
"CJ3Vo0AVXczYoLVUqc1+ACT+SJ4sMhvHIkz/UnKyb6lGLf2GunIFUbxi+Gs/4pSd\n" \
"/d2kH4v1QehaSvvXCbvXsA8E464udPyauDlgvp1S3oIqslWevhmmQjD6aQykzcoR\n" \
"IKkJSj+GHrCHwYPC6CBKWQKBgG1C3fxUBCG0UNcnzCRaafNWacjZEOKAODVRCAxh\n" \
"hxNIN6UbgaSoOa9o54MqJXO806goR4H1Wwmxb+F43iQuI7+DTLK3oMthAgyw5KXs\n" \
"7nEXoUO/E2FQY4YTi+gjp9r6rEP9nJnuOo1ftsY423CCnK6sx/RwrFm4AMUdFme0\n" \
"WB3tAoGAOOj5pCYWFwg2pt6VqtEdLQgcrzZ7N4kSzb4li4Pa6T3q/eU/zueQ2I26\n" \
"16i3gXIapyLnVeu/C+gGjW9BAKuB04DOdkqAeA8zeaDJNFQoluRh0BCFiLXA/8sm\n" \
"4IoxtqOGI1pM6+U9mO66Y1jqIsxNobO8j/00lBkIaAoY6i89NVE=\n" \
"-----END RSA PRIVATE KEY-----"
;

int led_pin = 10;
int led_state = 1;

void updateLedState(int desired_led_state) {
  printf("change led_state to %d\r\n", desired_led_state);
  led_state = desired_led_state;
  digitalWrite(led_pin, led_state);

  sprintf(publishPayload, "{\"state\":{\"reported\":{\"led\":%d}},\"clientToken\":\"%s\"}",
    led_state,
    clientId
  );
  printf("Publish [%s] %s\r\n", topic_publish, publishPayload);
  client.publish(topic_publish, publishPayload);
}

void checkLedState() {
  printf("try to get led_state\r\n");
  
  sprintf(publishPayload, "{\"state\":{\"reported\":{\"led\":%d}},\"clientToken\":\"%s\"}",
    led_state,
    clientId
  );
  printf("Publish [%s] %s\r\n", publishGetTopic, publishPayload);
  client.publish(publishGetTopic, publishPayload);

  // After publish "get" command AWS IoT would send "get/accepted" message and we can check led state in callback
}

void callback(char* topic, byte* payload, unsigned int length) {
  char buf[MQTT_MAX_PACKET_SIZE];
  char *pch;
  int desired_led_state;

  strncpy(buf, (const char *)payload, length);
  buf[length] = '\0';
  printf("Message arrived [%s] %s\r\n", topic, buf);

  if ((strstr(topic, "/shadow/update/accepted") != NULL) || (strstr(topic, "/shadow/get/accepted") != NULL)) {
    // payload format: {"state":{"reported":{"led":1},"desired":{"led":0}},"metadata":{"reported":{"led":{"timestamp":1466996558}},"desired":{"led":{"timestamp":1466996558}}},"version":7,"timestamp":1466996558}
    pch = strstr(buf, "\"desired\":{\"led\":");
    if (pch != NULL) {
      pch += strlen("\"desired\":{\"led\":");
      desired_led_state = *pch - '0';
      if (desired_led_state != led_state) {
        updateLedState(desired_led_state);
      }
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientId)) {
      Serial.println("connected");

      for (int i=0; i<5; i++) {
        printf("subscribe [%s]\r\n", subscribeTopic[i]);
        client.subscribe(subscribeTopic[i]);
      }

      checkLedState();
      updateLedState(led_state);

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, led_state);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    if (status == WL_CONNECTED) break;
    // retry after 1 second
    delay(1000);
  }

  wifiClient.setRootCA((unsigned char*)rootCABuff);
  wifiClient.setClientCertificate((unsigned char*)certificateBuff, (unsigned char*)privateKeyBuff);

  client.setServer(mqttServer, 8883);
  client.setCallback(callback);

  // For publish qos1 that server will send ack
  client.setPublishQos(MQTTQOS1);

  // For subscribe or publish, wait ack can keep command sequence in order
  client.waitForAck(true);

  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
